/*
 * blockQueue.c
 *
 *  Created on: 2017Äê4ÔÂ7ÈÕ
 *      Author: tupers
 */

#include "blockQueue.h"
#include <xdc/std.h>

int blockQueue_init(blockQueueObj* obj,const int num,const int blockSize)
{
	if(num<2)
		return BLOCKQUEUE_SIZEERR;
	memset(obj,0,sizeof(blockQueueObj));
	obj->queueSize = num;
	obj->blockSize = blockSize;
	obj->data = (unsigned char*)malloc(obj->queueSize*obj->blockSize);
	if(obj->data==NULL)
		return BLOCKQUEUE_ALLOCERR;
	obj->memberArray = (blockQueueMember*)malloc(sizeof(blockQueueMember)*num);
	if(obj->data==NULL)
		return BLOCKQUEUE_ALLOCERR;
	int i;
	for(i=0;i<num;i++)
	{
		obj->memberArray[i].data=obj->data+i*blockSize;
		obj->memberArray[i].used=0;
		if(i==0)
		{
			obj->memberArray[i].front=NULL;
			obj->memberArray[i].rear=&obj->memberArray[i+1];
		}
		else if(i==num-1)
		{
			obj->memberArray[i].front=&obj->memberArray[i-1];
			obj->memberArray[i].rear=NULL;
		}
		else
		{
			obj->memberArray[i].front=&obj->memberArray[i-1];
			obj->memberArray[i].rear=&obj->memberArray[i+1];
		}
	}
	obj->headMember=&obj->memberArray[0];
	obj->lastMember=&obj->memberArray[num-1];
	return BLOCKQUEUE_OK;
}

blockQueueMember* blockQueue_getEmetyBlock(blockQueueObj* obj)
{
	blockQueueMember* probe = obj->headMember;
	while(probe->rear!=NULL)
	{
		if(probe->used==0)
			return probe;
		probe = probe->rear;
	}
	return NULL;
}

blockQueueMember* blockQueue_getBlock(blockQueueObj* obj)
{
	int i;
	blockQueueMember* probe = obj->headMember;
	if(probe->used==0)
		return NULL;
	obj->headMember=probe->rear;
	obj->headMember->front=NULL;
	return probe;
}

void blockQueue_releaseBlock(blockQueueObj* obj,blockQueueMember* member)
{
	member->used=0;
	member->front=obj->lastMember;
	member->rear=NULL;
	obj->lastMember->rear=member;
	obj->lastMember=member;
}

