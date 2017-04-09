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

#include "edma.h"

#define BLOCKSIZE       8192
#define BLOCKQUEUENUM  5

#define BLOCKALG_OK         0
#define BLOCKALG_SIZEERR    1
#define BLOCKALG_ALLOCERR   2

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
    blockQueueStr blockQueue;
    edma_object_t inL2Obj;
    edma_object_t outL2Obj;
    blockDataStr blockData;
    Task_Handle inL2Task;
    Task_Handle outL2Tast;
}blockAlgObject;

int blockAlg_init(int width,int height,blockAlgObject* handle);
int blockAlg_blockDivide(int width,int height,blockAlgObject* handle);
void blockAlg_inL2Task(UArg arg0,UArg arg1);

#endif /* BLOCKALG_H_ */
