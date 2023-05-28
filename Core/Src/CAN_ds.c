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

void initQueue(Queue* q, S_COImessage* data_arr, uint8_t size)
{
	q->QSize 	= size;
	q->data 	= data_arr;
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

void enqueue(Queue* q, S_COImessage* element)
{

    if (isQueueFull(q)) {
    	S_COImessage* _;
    	dequeue(q, _);
    }

    if (isQEmpty(q)) {
        q->front = 0;
        q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % q->QSize;
    }

    q->data[q->rear] = *element;
}

void dequeue(Queue* q, S_COImessage* msg)
{

    if (isQEmpty(q)) {
        return;
    }

    msg = &q->data[q->front];

    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % q->QSize;
    }

}
