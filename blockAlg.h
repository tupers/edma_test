/*
 * blockAlg.h
 *
 *  Created on: 2017Äê4ÔÂ9ÈÕ
 *      Author: glamo
 */

#ifndef BLOCKALG_H_
#define BLOCKALG_H_

#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Mailbox.h>

#include "edma.h"


#define BLOCKSIZE       38500
#define BLOCKQUEUENUM  	3
#define BLOCKMAILBOXNUM	50

#define BLOCKALG_OK         0
#define BLOCKALG_SIZEERR    1
#define BLOCKALG_ALLOCERR   2

typedef struct
{
	Queue_Handle queue;
	Queue_Elem* elem;
}blockEdmaCbData;

typedef struct
{
	void* src;
	void* dst;
	unsigned short width;
	unsigned short height;
	int isExit;//true 1 false 0
	Queue_Elem* elem;
}blockMailboxMsg;

typedef struct
{
   unsigned char* data;
   int isFull;//true 1 ,false 0
}blockElementInfoStr;


typedef struct
{
    Queue_Elem elem;
    blockElementInfoStr block;
}blockTransferElementStr;

typedef struct
{
    Queue_Handle fullQueue;
    Queue_Handle emptyQueue;
    blockTransferElementStr* elemArray;
}blockQueueStr;

typedef struct
{
    unsigned short cols;
    unsigned short rows;
    unsigned short blockRows;
    unsigned short cycleTimes;
    unsigned int totalSize;
    unsigned int blockSize;
    unsigned int lastBlockSize;
    unsigned char* data;
}blockDataStr;

typedef struct
{
	Task_Handle task;
	Mailbox_Handle mailbox;
	edma_object_t edmaObj;
}blockAlgTaskStr;
typedef struct
{
    blockQueueStr blockQueue;
    blockDataStr blockData;
    blockAlgTaskStr inL2Tsk;
    blockAlgTaskStr outL2Tsk;
}blockAlgObject;

int blockAlg_init(int width,int height,blockAlgObject* handle);
int blockAlg_blockDivide(int width,int height,blockAlgObject* handle);
void blockAlg_inL2Task(UArg arg0,UArg arg1);
void blockAlg_edmaIsr(uint32_t tcc, EDMA3_RM_TccStatus status, void *cbData);
void blockAlg_process(const unsigned char* inData,blockAlgObject* handle);
blockTransferElementStr* blockAlg_getElem(Queue_Handle handle);
void blockAlg_putElem(Queue_Handle handle,Queue_Elem* elem);
void blockAlg_deinit(blockAlgObject* handle);
#endif /* BLOCKALG_H_ */
