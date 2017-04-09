/*
 * blockAlg.c
 *
 *  Created on: 2017Äê4ÔÂ9ÈÕ
 *      Author: glamo
 */

#include "blockAlg.h"

int blockAlg_init(int width,int height,blockAlgObject* handle)
{

    memset(handle,0,sizeof(blockAlgObject));
    blockDataStr* blockData = &handle->blockData;

    //decide block num and cycle time
    if(blockAlg_blockDivide(width,height,handle)!=BLOCKALG_OK)
        return BLOCKALG_SIZEERR;

    //alloc L2 block buffers
    blockData->data = (unsigned char*)malloc(sizeof(unsigned char)*BLOCKQUEUENUM*blockData->blockSize);
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
        handle->blockQueue.elemArray[i].block.data = blockData->data+i*blockData->blockSize;
        Queue_enqueue(handle->blockQueue.emptyQueue,&(handle->blockQueue.elemArray[i].elem));
    }

    //create edma Task and edma instance
    Task_Params inL2TaskParams;
    handle->inL2Obj.edma3InstanceId=0;
    inL2TaskParams.arg0 = &handle->inL2Obj;
    inL2TaskParams.priority = 7;
    handle->inL2Task = Task_create(blockAlg_inL2Task,&inL2TaskParams,NULL);

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
    edma_object_t*obj = arg0;

}

