/*
 * blockAlg.c
 *
 *  Created on: 2017Äê4ÔÂ9ÈÕ
 *      Author: glamo
 */

#include "blockAlg.h"
unsigned char workbuf[115300];
#pragma DATA_SECTION(workbuf, ".workSpace");

int checkval = 0;

int blockAlg_init(int width,int height,blockAlgObject* handle)
{

    memset(handle,0,sizeof(blockAlgObject));
    blockDataStr* blockData = &handle->blockData;

    //decide block num and cycle time
    if(blockAlg_blockDivide(width,height,handle)!=BLOCKALG_OK)
        return BLOCKALG_SIZEERR;

    //alloc L2 block buffers
//    blockData->data = (unsigned char*)malloc(sizeof(unsigned char)*BLOCKQUEUENUM*blockData->blockSize);
    blockData->data = workbuf;
    if(blockData->data==NULL)
        return BLOCKALG_ALLOCERR;

    //alloc block queue element for transfer
    handle->blockQueue.elemArray = (blockTransferElementStr*)malloc(sizeof(blockTransferElementStr)*BLOCKQUEUENUM);
    if(handle->blockQueue.elemArray==NULL)
    {
        free(blockData->data);
        return BLOCKALG_ALLOCERR;
    }

    //create block queue
    handle->blockQueue.emptyQueue = Queue_create(NULL,NULL);
    handle->blockQueue.fullQueue = Queue_create(NULL,NULL);

    //init block queue with L2 Block buffers
    int i;
    for(i=0;i<BLOCKQUEUENUM;i++)
    {
        handle->blockQueue.elemArray[i].block.isFull=0;
        handle->blockQueue.elemArray[i].block.data = (blockData->data+i*blockData->blockSize);
        Queue_enqueue(handle->blockQueue.emptyQueue,&(handle->blockQueue.elemArray[i].elem));
    }

    //create task mailbox
    Mailbox_Params mBoxParams;
    Mailbox_Params_init(&mBoxParams);
    handle->inL2Tsk.mailbox = Mailbox_create(sizeof(blockMailboxMsg),BLOCKMAILBOXNUM,&mBoxParams,NULL);

    //create edma Task and edma instance
    Task_Params inL2TaskParams;
    Task_Params_init(&inL2TaskParams);
    handle->inL2Tsk.edmaObj.edma3InstanceId=0;
    inL2TaskParams.arg0 = (xdc_UArg)&handle->inL2Tsk;
    inL2TaskParams.arg1 = (xdc_UArg)&handle->blockQueue;
    inL2TaskParams.priority = 7;
    handle->inL2Tsk.task = Task_create(blockAlg_inL2Task,&inL2TaskParams,NULL);

    return BLOCKALG_OK;
}



int blockAlg_blockDivide(int width,int height,blockAlgObject* handle)
{
    if(width<1||height<1||width%8!=0||height%8!=0)
        return BLOCKALG_SIZEERR;
    blockDataStr* obj = &handle->blockData;
    obj->cols = width;
    obj->rows = height;
    obj->totalSize = width*height;
    int blockRows = BLOCKSIZE/obj->cols;
    if(blockRows>=obj->rows)
    {
        obj->blockSize = obj->totalSize;
        obj->cycleTimes = 1;
        obj->lastBlockSize = 0;
    }
    else
    {
        obj->blockSize = obj->cols * blockRows;
        obj->cycleTimes = obj->rows/blockRows;
        obj->lastBlockSize = (obj->rows%blockRows)*obj->cols;
    }
    obj->blockRows = blockRows;

    return BLOCKALG_OK;
}

void blockAlg_inL2Task(UArg arg0,UArg arg1)
{
	blockAlgTaskStr* tskObj = (blockAlgTaskStr*)arg0;
	blockQueueStr* queue = (blockQueueStr*)arg1;

	edma_object_t* edmaObj = &tskObj->edmaObj;
	Mailbox_Handle mailBox = tskObj->mailbox;
	blockEdmaCbData* cbData = (blockEdmaCbData*)malloc(sizeof(blockEdmaCbData));
    edma_config(edmaObj);
    edma_createChan(edmaObj,&blockAlg_edmaIsr,cbData);
    blockMailboxMsg msg;
    while(true)
    {
    	Mailbox_pend(mailBox,&msg,BIOS_WAIT_FOREVER);
    	if(msg.isExit==1)
    		break;
    	cbData->queue = queue->fullQueue;
    	cbData->elem = msg.elem;
    	edmaObj->params.aCnt = msg.width;
    	edmaObj->params.bCnt = msg.height;
    	edmaObj->params.cCnt = 1;
    	edmaObj->params.srcBIdx = msg.width;
    	edmaObj->params.destBIdx = msg.width;
    	edmaObj->params.srcCIdx = msg.width;
    	edmaObj->params.destCIdx = msg.width;
    	edma_setParam(edmaObj,msg.src,msg.dst);
    	edma_start(*edmaObj);
    }
    free(cbData);
    edma_deleteChan(*edmaObj);
    edma_release(edmaObj);
}

void blockAlg_edmaIsr(uint32_t tcc, EDMA3_RM_TccStatus status, void *cbData)
{
	blockEdmaCbData* data = cbData;
	Queue_enqueue(data->queue,data->elem);
}

blockTransferElementStr* blockAlg_getElem(Queue_Handle handle)
{
	while(Queue_empty(handle))
	{
		Task_sleep(500);
	}
	return Queue_dequeue(handle);
}

void blockAlg_putElem(Queue_Handle handle,Queue_Elem* elem)
{
	Queue_enqueue(handle,elem);
}

void blockAlg_process(const unsigned char* inData,blockAlgObject* handle)
{
	blockMailboxMsg msg;

	//first transfer
	blockTransferElementStr* emptyElem = blockAlg_getElem(handle->blockQueue.emptyQueue);
	msg.src = (unsigned char*)inData;
	msg.dst = emptyElem->block.data;
	msg.isExit = 0;
	msg.elem = &emptyElem->elem;
	msg.width = handle->blockData.cols;
	msg.height = handle->blockData.blockRows;
	Edma3_CacheWb((unsigned int)msg.src,handle->blockData.blockSize);
	Mailbox_post(handle->inL2Tsk.mailbox,&msg,BIOS_WAIT_FOREVER);

	//start block transfer
	blockTransferElementStr* L2Elem;
	int i;
	for(i=1;i<handle->blockData.cycleTimes;i++)
	{
		//get L2 transfered block
		L2Elem = blockAlg_getElem(handle->blockQueue.fullQueue);
		Edma3_CacheInv((unsigned int)L2Elem->block.data,handle->blockData.blockSize);

		//before process , transfer next block first
		emptyElem = blockAlg_getElem(handle->blockQueue.emptyQueue);
		msg.elem = &emptyElem->elem;
		msg.src = (unsigned char*)inData+i*handle->blockData.blockSize;
		msg.dst = emptyElem->block.data;
		Edma3_CacheWb((unsigned int)msg.src,handle->blockData.blockSize);
		Mailbox_post(handle->inL2Tsk.mailbox,&msg,BIOS_WAIT_FOREVER);

		//process size = blockSize
		int c;
		for(c=0;c<handle->blockData.blockSize;c++)
		{
			if(L2Elem->block.data[c]>0)
				checkval++;
		}

		//put used elem back to empty queue
		blockAlg_putElem(handle->blockQueue.emptyQueue,&L2Elem->elem);
	}
	if(handle->blockData.lastBlockSize>0)
	{
		//get L2 transfered block
		L2Elem = blockAlg_getElem(handle->blockQueue.fullQueue);
		Edma3_CacheInv((unsigned int)L2Elem->block.data,handle->blockData.blockSize);

		//before process , transfer last block first
		emptyElem = blockAlg_getElem(handle->blockQueue.emptyQueue);
		msg.elem = &emptyElem->elem;
		msg.src = (unsigned char*)inData+handle->blockData.cycleTimes*handle->blockData.blockSize;
		msg.dst = emptyElem->block.data;
		msg.height = handle->blockData.lastBlockSize/handle->blockData.cols;
		Edma3_CacheWb((unsigned int)msg.src,handle->blockData.lastBlockSize);
		Mailbox_post(handle->inL2Tsk.mailbox,&msg,BIOS_WAIT_FOREVER);

		//process size = blockSize
		int c;
		for(c=0;c<handle->blockData.blockSize;c++)
		{
			if(L2Elem->block.data[c]>0)
				checkval++;
		}


		//put used elem back to empty queue
		blockAlg_putElem(handle->blockQueue.emptyQueue,&L2Elem->elem);

		//get last L2 transfered block
		L2Elem = blockAlg_getElem(handle->blockQueue.fullQueue);
		Edma3_CacheInv((unsigned int)L2Elem->block.data,handle->blockData.lastBlockSize);

		//process size = lastBlockSize

		for(c=0;c<handle->blockData.lastBlockSize;c++)
		{
			if(L2Elem->block.data[c]>0)
				checkval++;
		}

		//put used elem back to empty queue
		blockAlg_putElem(handle->blockQueue.emptyQueue,&L2Elem->elem);
	}
	else
	{
		//get final L2 transfered block
		L2Elem = blockAlg_getElem(handle->blockQueue.fullQueue);
		Edma3_CacheInv((unsigned int)L2Elem->block.data,handle->blockData.blockSize);

		//process size = blockSize

		int c;
		for(c=0;c<handle->blockData.blockSize;c++)
		{
			if(L2Elem->block.data[c]>0)
				checkval++;
		}

		//put used elem back to empty queue
		blockAlg_putElem(handle->blockQueue.emptyQueue,&L2Elem->elem);
	}
}

void blockAlg_deinit(blockAlgObject* handle)
{
	//send end cmd to edma task
	blockMailboxMsg msg;
	msg.isExit=1;
	Mailbox_post(handle->inL2Tsk.mailbox,&msg,BIOS_WAIT_FOREVER);

	//free L2 resource
	Task_delete(&handle->inL2Tsk.task);
	Mailbox_delete(&handle->inL2Tsk.mailbox);
	Queue_delete(&handle->blockQueue.emptyQueue);
	Queue_delete(&handle->blockQueue.fullQueue);
	free(handle->blockQueue.elemArray);
//	free(handle->blockData.data);

}

