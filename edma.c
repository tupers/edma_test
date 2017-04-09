/*
 * edma.c
 *
 *  Created on: 2017Äê4ÔÂ9ÈÕ
 *      Author: glamo
 */


#include "edma.h"

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

void edma_createChan(edma_object_t *EdmaObj,EDMA3_RM_TccCallback cb,void* cbData)
{
    EDMA3_DRV_Result edma3Result = EDMA3_DRV_SOK; // return value for some driver calls so they can return an error
    EDMA3_RM_EventQueue eventQ = 0; // both used below in _requestChannel()
    edma3Result = EDMA3_DRV_requestChannel(EdmaObj->hEdma, &EdmaObj->iChannel,
            &EdmaObj->iTcc, eventQ, cb, cbData); // get channel for transfer - iChannel
//  For debug purposes, get currentParam set so that we can add it to a watch window for debug
//    edma3Result = EDMA3_DRV_getPaRAM(EdmaObj->hEdma, EdmaObj->iChannel,
//            &currentParam);
}

void edma_setParam(edma_object_t *EdmaObj,void *Src,void *Dst)
{
    EDMA3_DRV_Result edma3Result = EDMA3_DRV_SOK;
    edma3Result = EDMA3_DRV_setSrcParams(EdmaObj->hEdma, EdmaObj->iChannel,
            (unsigned int) (Src), EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT); // set source parameters - also assigns defaults to Options register
    edma3Result = EDMA3_DRV_setDestParams(EdmaObj->hEdma, EdmaObj->iChannel,
            (unsigned int) (Dst), EDMA3_DRV_ADDR_MODE_INCR, EDMA3_DRV_W8BIT); // set dest parameters - also assigns defaults to Options register

    edma3Result = EDMA3_DRV_setSrcIndex(EdmaObj->hEdma, EdmaObj->iChannel, EdmaObj->params.srcBIdx,
            EdmaObj->params.srcCIdx); // set SRCBIDX to 2, SRCCIDX to 0 (single AB transfer)
    edma3Result = EDMA3_DRV_setDestIndex(EdmaObj->hEdma, EdmaObj->iChannel, EdmaObj->params.destBIdx,
            EdmaObj->params.destCIdx); // set DSTBIDX to 2, DSTCIDX to 0

    edma3Result = EDMA3_DRV_setTransferParams(EdmaObj->hEdma, EdmaObj->iChannel,
            EdmaObj->params.aCnt, EdmaObj->params.bCnt, EdmaObj->params.cCnt, 0, EDMA3_DRV_SYNC_AB); //ACNT=2, BCNT=BUFFSIZE, CCNT=1, BCNTRLD=0, SYNC=AB
    edma3Result = EDMA3_DRV_setOptField(EdmaObj->hEdma, EdmaObj->iChannel,
            EDMA3_DRV_OPT_FIELD_TCINTEN, EDMA3_DRV_TCINTEN_EN); //Interrupt enable DISABLED - default is DIS if not called
    edma3Result = EDMA3_DRV_setOptField(EdmaObj->hEdma, EdmaObj->iChannel,
            EDMA3_DRV_OPT_FIELD_ITCINTEN, EDMA3_DRV_ITCINTEN_EN);
    edma3Result = EDMA3_DRV_setOptField(EdmaObj->hEdma, EdmaObj->iChannel,
            EDMA3_DRV_OPT_FIELD_TCC, EdmaObj->iTcc);
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
