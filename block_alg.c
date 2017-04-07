/*
 * block_alg.c
 *
 *  Created on: 2017Äê4ÔÂ7ÈÕ
 *      Author: tupers
 */

#include "block_alg.h"

unsigned char workBuf[BLOCKSIZE];
#pragma DATA_SECTION(workBuf, ".workSpace")
//unsigned char dst[1920*1080];
//#pragma DATA_SECTION()

extern signed char* getGlobalAddr(signed char* addr);
#define GLOBAL_ADDR(addr) (getGlobalAddr(addr))

alg_struct myAlg;

void algInput_edmaIsr(uint32_t tcc, EDMA3_RM_TccStatus status, void *appData)
{
	edma3OsSemGive(appData);
}
void algOutput_edmaIsr(uint32_t tcc, EDMA3_RM_TccStatus status, void *appData)
{
	edma3OsSemGive(appData);
}
int alg_init(int width, int height, alg_struct* handle)
{
	_nassert((int) width % 8 == 0);
	_nassert((int) height % 8 == 0);
	memset(handle, 0, sizeof(alg_struct));
	handle->cols = width;
	handle->rows = height;
	int blockRow = BLOCKSIZE / width;
	if (blockRow >= handle->rows)
	{
		handle->blockSize = handle->cols * handle->rows;
		handle->totalSize = handle->blockSize;
		handle->cycleTimes = 1;
		handle->lastBlockSize = 0;
	} else
	{
		handle->blockSize = handle->cols * blockRow;
		handle->totalSize = handle->cols * handle->rows;
		handle->cycleTimes = handle->rows / blockRow;
		handle->lastBlockSize = (handle->rows % blockRow) * handle->cols;
	}
	handle->work = workBuf;
	handle->inputIsr = &algInput_edmaIsr;
	handle->outputIsr = &algOutput_edmaIsr;

	handle->edma_input.edma3InstanceId = 0;
	handle->edma_output.edma3InstanceId = 1;
	edma_config(&handle->edma_input);
	edma_config(&handle->edma_output);

	return 0;
}

void alg_process(const unsigned char* src, alg_struct* handle)
{
	if (handle->cycleTimes > 0)
	{
		Edma3_CacheWb((unsigned int) src, handle->blockSize);
		alg_copy(src, handle->work, handle->cols,
				handle->blockSize / handle->cols, &handle->edma_input,
				handle->inputIsr);
//		edma3OsSemGive(handle->edma_output.hEdmaSem);
	}

	if (handle->cycleTimes > 0)
	{
		int i;
		for (i = 1; i < handle->cycleTimes; i++)
		{
			unsigned char* pSrc = (unsigned char*) src + BLOCKSIZE * i;
			edma3OsSemTake(handle->edma_input.hEdmaSem, EDMA3_OSSEM_NO_TIMEOUT);
			edma_deleteChan(handle->edma_input);
			Edma3_CacheInv((unsigned int) handle->work, handle->blockSize);
			Edma3_CacheWb((unsigned int) pSrc, handle->blockSize);
			alg_copy(pSrc, handle->work, handle->cols,
					handle->blockSize / handle->cols, &handle->edma_input,
					handle->inputIsr);
			//do algorithm size = blockSize;
//			edma3OsSemTake(handle->edma_output.hEdmaSem,EDMA3_OSSEM_NO_TIMEOUT);
//			Edma3_CacheWb((unsigned int)handle->work,handle->blockSize)
//			alg_copy()

		}
	}
	if (handle->lastBlockSize > 0)
	{
		unsigned char* pSrc = (unsigned char*) src
				+ BLOCKSIZE * handle->cycleTimes;
		edma3OsSemTake(handle->edma_input.hEdmaSem, EDMA3_OSSEM_NO_TIMEOUT);
		edma_deleteChan(handle->edma_input);
		Edma3_CacheInv((unsigned int) handle->work, handle->blockSize);
		Edma3_CacheWb((unsigned int) pSrc, handle->lastBlockSize);
		alg_copy(pSrc, handle->work, handle->cols,
				handle->lastBlockSize / handle->cols, &handle->edma_input,
				handle->inputIsr);
		//do algorithm size = blockSize;
	}
	edma3OsSemTake(handle->edma_input.hEdmaSem, EDMA3_OSSEM_NO_TIMEOUT);
	edma_deleteChan(handle->edma_input);
	Edma3_CacheInv((unsigned int) handle->work, handle->lastBlockSize);
	//do last algorithm size = lastBlockSize
}

void alg_copy(const unsigned char* src, unsigned char* dst, int width,
		int height, edma_object_t*EdmaObj, EDMA3_RM_TccCallback cb)
{
	int size = width * height;
	unsigned char* srcBuf = (unsigned char*) GLOBAL_ADDR((signed char * )src);
	unsigned char* dstBuf = (unsigned char*) GLOBAL_ADDR((signed char * )dst);
	if (size >= 1024)
	{
		EdmaObj->Acnt = width;
		EdmaObj->Bcnt = height;
		EdmaObj->Ccnt = 1;
		EdmaObj->SrcBIDX = width;
		EdmaObj->DstBIDX = width;
		EdmaObj->SrcCIDX = width;
		EdmaObj->DstCIDX = width;
	}
	edma_createChan(EdmaObj, srcBuf, dstBuf, cb, EdmaObj->hEdmaSem);
	edma_start(*EdmaObj);
}

