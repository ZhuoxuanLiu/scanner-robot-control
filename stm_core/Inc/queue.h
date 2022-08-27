/*
 * queue.h
 *
 *  Created on: 2022年8月25日
 *      Author: a3352
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include "main.h"
#include "stdlib.h"

typedef uint8_t *QDataType;

typedef struct QueueNode
{
	struct QueueNode* next;
	QDataType data;
}QNode;

typedef struct Queue//
{
	QNode* tail;//头指针
	QNode* head;//尾指针
}Queue;

void QueueInit(Queue* pq);

void QueuePush(Queue* pq, QDataType x);

void QueuePop(Queue* pq);

QDataType QueueFront(Queue* pq);

QDataType QueueBack(Queue* pq);

uint16_t QueueEmpty(Queue* pq);

void QueueDestory(Queue* pq);

extern Queue htim6_queue;
extern Queue htim7_queue;
extern Queue extra_queue;

#endif /* INC_QUEUE_H_ */
