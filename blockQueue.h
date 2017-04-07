/*
 * blockQueue.h
 *
 *  Created on: 2017Äê4ÔÂ7ÈÕ
 *      Author: tupers
 */

#ifndef BLOCKQUEUE_H_
#define BLOCKQUEUE_H_

#define BLOCKQUEUE_OK		0
#define BLOCKQUEUE_SIZEERR	1
#define BLOCKQUEUE_ALLOCERR	2

typedef struct
{
	unsigned char* data;
	int used;//unused 0,full 1
	void* front;
	void* rear;
}blockQueueMember;

typedef struct
{
	int queueSize;
	int blockSize;
	unsigned char* data;
	blockQueueMember* memberArray;
	blockQueueMember* headMember;
	blockQueueMember* lastMember;
}blockQueueObj;

int blockQueue_init(blockQueueObj* obj,const int num,const int blockSize);
blockQueueMember* blockQueue_getEmetyBlock(blockQueueObj* obj);
blockQueueMember* blockQueue_getBlock(blockQueueObj* obj);
void blockQueue_releaseBlock(blockQueueObj* obj,blockQueueMember* member);



#endif /* BLOCKQUEUE_H_ */
