/*
 * edma.h
 *
 *  Created on: 2017Äê4ÔÂ9ÈÕ
 *      Author: glamo
 */

#ifndef EDMA_H_
#define EDMA_H_

#include <ti/sdo/edma3/drv/edma3_drv.h>
#include <ti/sdo/edma3/drv/sample/bios6_edma3_drv_sample.h>
#include <ti/sysbios/hal/Cache.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>


#define GLOBAL_ADDR(addr) (getGlobalAddr(addr))
extern signed char*  getGlobalAddr(signed char* addr);


typedef struct edma_object_t
{
    EDMA3_DRV_Handle hEdma;
    unsigned int iChannel;  // channel handle
    unsigned int iTcc;// Tcc
//    EDMA3_OS_Sem_Handle hEdmaSem;
    unsigned short edma3InstanceId;
    EDMA3_DRV_PaRAMRegs params;
} edma_object_t;

void edma_createChan(edma_object_t *EdmaObj,EDMA3_RM_TccCallback cb,void* cbData);
void edma_setParam(edma_object_t *EdmaObj,void *Src,void *Dst);
int Edma3_CacheWb(unsigned int buf,unsigned int size);
int Edma3_CacheInv(unsigned int buf,unsigned int size);
void edma_start(edma_object_t EdmaObj);
void edma_wait(edma_object_t EdmaObj);
void edma_deleteChan(edma_object_t EdmaObj);
void edma_release(edma_object_t* EdmaObj);
void edma_config(edma_object_t* EdmaObj);

#endif /* EDMA_H_ */
