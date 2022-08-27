/*
 * queue.c
 *
 *  Created on: 2022年8月25日
 *      Author: a3352
 */


#include "queue.h"

Queue htim6_queue;
Queue htim7_queue;
Queue extra_queue;

void QueueInit(Queue* pq)
{
	pq->head = pq->tail = NULL;
}

void QueuePush(Queue* pq, QDataType x)
{
	QNode* newnode = (QNode*)malloc(sizeof(QNode));
    for (int i=0; i<4; i++)
    {
        newnode->data[i] = x[i];
    }
	newnode->next = NULL;
	if (pq->tail == NULL)
	{
		pq->head = pq->tail = newnode;
	}
	else
	{
		pq->tail->next = newnode;
		pq->tail = newnode;
	}
}


void QueuePop(Queue* pq)
{
	// 1、一个
	// 2、多个
	if (pq->head->next == NULL)
	{
		free(pq->head);
		pq->head = pq->tail = NULL;
	}
	else
	{
		QNode* next = pq->head->next;
		free(pq->head);
		pq->head = next;
	}
}


QDataType QueueFront(Queue* pq)
{
	return pq->head->data;
}

QDataType QueueBack(Queue* pq)
{
	return pq->tail->data;
}

uint16_t QueueEmpty(Queue* pq)
{
	if (pq->head == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void QueueDestory(Queue* pq)
{
	QNode* cur = pq->head;
	while (cur)
	{
		QNode* next = cur->next;
		free(cur);
		cur = next;
	}

	pq->head = pq->tail = NULL;
}
