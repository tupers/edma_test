/*
 * block_alg.h
 *
 *  Created on: 2017Äê4ÔÂ7ÈÕ
 *      Author: tupers
 */

#ifndef BLOCK_ALG_H_
#define BLOCK_ALG_H_

#define BLOCKSIZE 32768
#define BLOCKACNT 1024

#include "edma.h"

typedef struct
{
	int totalSize;
	int blockSize;
	int cycleTimes;
	int lastBlockSize;
	int cols;
	int rows;
	edma_object_t edma_input;
	edma_object_t edma_output;
	EDMA3_RM_TccCallback inputIsr;
	EDMA3_RM_TccCallback outputIsr;
	unsigned char* src;
	unsigned char* dst;
	unsigned char* work;
}alg_struct;

int alg_init(int width,int height,alg_struct* handle);
void alg_process(const unsigned char* src,alg_struct* handle);
void alg_copy(const unsigned char* src, unsigned char* dst, int width,int height,edma_object_t*EdmaObj,EDMA3_RM_TccCallback cb);
void algInput_edmaIsr(uint32_t tcc, EDMA3_RM_TccStatus status, void *appData);
void algOutput_edmaIsr(uint32_t tcc, EDMA3_RM_TccStatus status, void *appData);

#endif /* BLOCK_ALG_H_ */
