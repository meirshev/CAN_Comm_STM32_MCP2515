/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CAN_ds.c
  * @brief          : Contains all the data structures relevant for the
  * 				  CAN communication application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include <CAN_ds.h>

void initCommConfig(CommConfig* nodes, uint8_t* selfID)
{
	*selfID = NO_ID;

	for (int i=0; i < NUM_OF_NODES; i++)
	{
		nodes->commNodes[i] = false;
	}
}

void initQueue(Queue* q, S_COImessage* msgsArr, uint8_t size)
{
	q->QSize 	= size;
	q->msgsArr 	= msgsArr;
    q->front 	= -1;
    q->rear 	= -1;
}


bool isQueueEmpty(Queue* q)
{
    return (q->front == -1 && q->rear == -1);
}

bool isQueueFull(Queue* q)
{
    return ((q->rear + 1) % q->QSize == q->front);
}

/**
 * @Brief description of the function.
 *
 * Detailed description of the function and its purpose.
 *
 * @param q
 * @param element this argument is deep copied into memory in the heap.
 * @return
 */
void enqueue(Queue* q, S_COImessage* element)
{

    if (isQueueFull(q)) {
    	S_COImessage* _ = NULL;
    	dequeue(q, _);
    }

    if (isQueueEmpty(q)) {
        q->front = 0;
        q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % q->QSize;
    }

    q->msgsArr[q->rear].params.timeStamp 	= element->params.timeStamp;
    q->msgsArr[q->rear].params.J 			= element->params.J;
    q->msgsArr[q->rear].params.omega 		= element->params.omega;
}

int dequeue(Queue* q, S_COImessage* msg)
{

    if (isQueueEmpty(q)) {
        return 0;
    }

    msg->params.timeStamp 	= q->msgsArr[q->front].params.timeStamp;
    msg->params.J 			= q->msgsArr[q->front].params.J;
    msg->params.omega 		= q->msgsArr[q->front].params.omega;

    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % q->QSize;
    }

    return 1;
}
