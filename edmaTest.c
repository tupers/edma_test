/*
 * edmaTest.c
 *
 *  Created on: 2017年3月6日
 *      Author: tupers
 */

#include "edma.h"

short dst[32];
short src[32];
//EDMA3_OS_Sem_Handle mySem;


void edma_input_isr(uint32_t tcc, EDMA3_RM_TccStatus status, void *appData)
{
	(void) tcc;
	(void) appData;

//	System_printf("call back here.\n");
	switch (status)
	{
		case EDMA3_RM_XFER_COMPLETE:
			/* Transfer completed successfully */
//			System_printf("success.\n");
			break;
		case EDMA3_RM_E_CC_DMA_EVT_MISS:
			/* Transfer resulted in DMA event miss error. */
//			System_printf("dma event miss.\n");
			break;
		case EDMA3_RM_E_CC_QDMA_EVT_MISS:
			/* Transfer resulted in QDMA event miss error. */
//			System_printf("qdma event miss.\n");
			break;
		default:
			break;
	}
//	edma3OsSemGive(mySem);
}
extern EDMA3_DRV_GblConfigParams sampleEdma3GblCfgParams;
extern EDMA3_DRV_InstanceInitConfig sampleInstInitConfig;

void edma_config(edma_object_t* EdmaObj)
{
	EDMA3_DRV_Result edmaResult = EDMA3_DRV_SOK;
	Semaphore_Params semParams;
	Semaphore_Params_init(&semParams);
	edmaResult = edma3OsSemCreate(EdmaObj->edma3InstanceId, &semParams, &EdmaObj->hEdmaSem);
	EdmaObj->hEdma=NULL;
	EdmaObj->iChannel=EDMA3_DRV_DMA_CHANNEL_ANY;
	EdmaObj->iTcc=EDMA3_DRV_TCC_ANY;
	EdmaObj->hEdma = edma3init(EdmaObj->edma3InstanceId,&edmaResult);
}

void edma_createChan(edma_object_t *EdmaObj, void *Src, void *Dst,EDMA3_RM_TccCallback cb,void* cbData)
{
	EDMA3_DRV_Result edma3Result = EDMA3_DRV_SOK; // return value for some driver calls so they can return an error
	EDMA3_DRV_PaRAMRegs currentParam; // used for debug purposes in EDMA3_DRV_getPaRAM()

	EDMA3_RM_EventQueue eventQ = 0; // both used below in _requestChannel()
	//EDMA3_RM_TccCallback tccCb = &edma_input_isr;

	edma3Result = EDMA3_DRV_requestChannel(EdmaObj->hEdma, &EdmaObj->iChannel,
			&EdmaObj->iTcc, eventQ, cb, cbData); // get channel for transfer - iChannel

	edma3Result = EDMA3_DRV_setSrcParams(EdmaObj->hEdma, EdmaObj->iChannel,
			(unsigned int) (Src), EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT); // set source parameters - also assigns defaults to Options register
	edma3Result = EDMA3_DRV_setDestParams(EdmaObj->hEdma, EdmaObj->iChannel,
			(unsigned int) (Dst), EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT); // set dest parameters - also assigns defaults to Options register

	edma3Result = EDMA3_DRV_setSrcIndex(EdmaObj->hEdma, EdmaObj->iChannel, EdmaObj->SrcBIDX,
			EdmaObj->SrcCIDX); // set SRCBIDX to 2, SRCCIDX to 0 (single AB transfer)
	edma3Result = EDMA3_DRV_setDestIndex(EdmaObj->hEdma, EdmaObj->iChannel, EdmaObj->DstBIDX,
			EdmaObj->DstCIDX); // set DSTBIDX to 2, DSTCIDX to 0

	edma3Result = EDMA3_DRV_setTransferParams(EdmaObj->hEdma, EdmaObj->iChannel,
			EdmaObj->Acnt, EdmaObj->Bcnt, EdmaObj->Ccnt, 0, EDMA3_DRV_SYNC_AB); //ACNT=2, BCNT=BUFFSIZE, CCNT=1, BCNTRLD=0, SYNC=AB

	edma3Result = EDMA3_DRV_getPaRAM(EdmaObj->hEdma, EdmaObj->iChannel,
			&currentParam);
	edma3Result = EDMA3_DRV_setOptField(EdmaObj->hEdma, EdmaObj->iChannel,
			EDMA3_DRV_OPT_FIELD_TCINTEN, EDMA3_DRV_TCINTEN_EN); //Interrupt enable DISABLED - default is DIS if not called
	edma3Result = EDMA3_DRV_setOptField(EdmaObj->hEdma, EdmaObj->iChannel,
			EDMA3_DRV_OPT_FIELD_ITCINTEN, EDMA3_DRV_ITCINTEN_EN);
	edma3Result = EDMA3_DRV_setOptField(EdmaObj->hEdma, EdmaObj->iChannel,
			EDMA3_DRV_OPT_FIELD_TCC, EdmaObj->iTcc);
//  For debug purposes, get currentParam set so that we can add it to a watch window for debug
	edma3Result = EDMA3_DRV_getPaRAM(EdmaObj->hEdma, EdmaObj->iChannel,
			&currentParam);
}

int Edma3_CacheWb(unsigned int buf, unsigned int size)
{
	Cache_wb((Ptr) buf, size, Cache_Type_ALL, TRUE);

	return 0;
}

int Edma3_CacheInv(unsigned int buf, unsigned int size)
{
	Cache_inv((Ptr) buf, size, Cache_Type_ALL, TRUE);

	return 0;
}
void edma_start(edma_object_t EdmaObj)
{
	EDMA3_DRV_Result edma3Result = EDMA3_DRV_SOK;
	edma3Result = EDMA3_DRV_enableTransfer(EdmaObj.hEdma, EdmaObj.iChannel,
			EDMA3_DRV_TRIG_MODE_MANUAL);
}

void edma_wait(edma_object_t EdmaObj)
{
	EDMA3_DRV_Result edma3Result = EDMA3_DRV_SOK;
	edma3Result = EDMA3_DRV_waitAndClearTcc(EdmaObj.hEdma, EdmaObj.iTcc);
}

void edma_deleteChan(edma_object_t EdmaObj)
{
	EDMA3_DRV_Result edma3Result = EDMA3_DRV_SOK;
	edma3Result = EDMA3_DRV_freeChannel(EdmaObj.hEdma, EdmaObj.iChannel);
}

void edma_release(edma_object_t* EdmaObj)
{
	EDMA3_DRV_Result edma3Result = EDMA3_DRV_SOK;
	edma3Result = edma3OsSemDelete(EdmaObj->hEdmaSem);
	edma3Result = EDMA3_DRV_close(EdmaObj->hEdma, NULL);
	edma3Result = EDMA3_DRV_delete(EdmaObj->edma3InstanceId, NULL);
}

//void edmaTest()
//{
//	EDMA3_DRV_Result edmaResult = EDMA3_DRV_SOK;
//	Semaphore_Params semParams;
//	Semaphore_Params_init(&semParams);
//////	const unsigned char* src = srcimg;
//	memset(dst,1,sizeof(short)*32);
//	memset(src,0,sizeof(short)*32);
//	short* srcBuf = (short*) GLOBAL_ADDR((char *)src);
//	short* dstBuf = (short*) GLOBAL_ADDR((char *)dst);
//#if 0
//    if (edmaResult == EDMA3_DRV_SOK)
//    {
//    	edmaResult = Edma3_CacheFlush((uint32_t)src, 32*sizeof(short));
//    }
//
//    /* Invalidate the Destination Buffer */
//    if (edmaResult == EDMA3_DRV_SOK)
//    {
//    	edmaResult = Edma3_CacheInvalidate((uint32_t)dst, 32*sizeof(short));
//    }
//#endif
//
//    /*
//    * Note: These functions are required if the buffer is in DDR.
//    * For other cases, where buffer is NOT in DDR, user
//    * may or may not require the below functions.
//    */
//    /* Flush the Source Buffer */
//    if (edmaResult == EDMA3_DRV_SOK)
//	{
//    	edmaResult = Edma3_CacheWb((unsigned int)src, 64);
//	}
//
//	edmaResult = edma3OsSemCreate(0, &semParams, &mySem);
//
//	edma_object_t EdmaObj={NULL,EDMA3_DRV_DMA_CHANNEL_ANY,EDMA3_DRV_TCC_ANY};
//	//edma_config(&EdmaObj);
//	EdmaObj.hEdma = edma3init(0,&edmaResult);
//	edma_createChan(&EdmaObj, srcBuf, dstBuf);
//	edma_start(EdmaObj);
//	edma3OsSemTake(mySem,EDMA3_OSSEM_NO_TIMEOUT);
////	edma_wait(EdmaObj);
//    /* Invalidate the Destination Buffer */
//    if (edmaResult == EDMA3_DRV_SOK)
//	{
//    	edmaResult = Edma3_CacheInv((unsigned int)dst, 64);
//	}
//    System_printf("%d,%d,%d,%d\n",dst[0],dst[1],dst[2],dst[3]);
//	edma_deleteChan(EdmaObj);
//}

