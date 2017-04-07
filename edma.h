/*
 * edma.h
 *
 *  Created on: 2017Äê4ÔÂ7ÈÕ
 *      Author: tupers
 */


#ifndef EDMA_H_
#define EDMA_H_

#include <ti/sdo/edma3/drv/edma3_drv.h>
#include <ti/sdo/edma3/drv/sample/bios6_edma3_drv_sample.h>
#include <ti/sysbios/hal/Cache.h>
#include <xdc/runtime/System.h>



typedef struct edma_object_t
{
	EDMA3_DRV_Handle hEdma;
	unsigned int iChannel;	// channel handle
	unsigned int iTcc;// Tcc
	EDMA3_OS_Sem_Handle hEdmaSem;
	unsigned short edma3InstanceId;
	unsigned short Acnt;
	unsigned short Bcnt;
	unsigned short Ccnt;
	unsigned short SrcBIDX;
	unsigned short DstBIDX;
	unsigned short SrcCIDX;
	unsigned short DstCIDX;
} edma_object_t;

void edma_input_isr(uint32_t tcc, EDMA3_RM_TccStatus status, void *appData);
void edma_output_isr(uint32_t tcc,EDMA3_RM_TccStatus status, void *appData);
void edma_createChan(edma_object_t *EdmaObj, void *Src, void *Dst,EDMA3_RM_TccCallback cb,void* cbData);
int Edma3_CacheWb(unsigned int buf,unsigned int size);
int Edma3_CacheInv(unsigned int buf,unsigned int size);
void edma_start(edma_object_t EdmaObj);
void edma_wait(edma_object_t EdmaObj);
void edma_deleteChan(edma_object_t EdmaObj);
void edma_release(edma_object_t* EdmaObj);
void edma_config(edma_object_t* EdmaObj);




#endif /* EDMA_H_ */
