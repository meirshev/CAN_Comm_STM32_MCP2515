/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CAN_ds.h
  * @brief          : Contains all the data structures relevant for the
  * 				  application.
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

#ifndef INC_CAN_DS_H_
#define INC_CAN_DS_H_

#include "stm32h7xx_hal.h"

#define true 	1
#define false 	0

typedef int bool;

typedef struct{

	uint32_t 	timeStamp;
	float 		J;
	float		omega;

}S_COImessage;

enum IDS{

	CENTERAL_NODE	= 0,
	NODE_1 			= 1,
	NODE_2			= 2,
	NODE_3			= 3,
	NODE_4			= 4,
	NODE_5			= 5,
	NODE_6			= 6

};

typedef struct{

	uint8_t selfID;

	uint8_t id0;
	uint8_t id1;
	uint8_t id2;
	uint8_t id3;
	uint8_t id4;
	uint8_t id5;

}IDs;


typedef struct {
	S_COImessage 	*data;
	uint8_t		 	QSize;
    int 			front;
    int 			rear;
} Queue;


void 			initIDsList(IDs* ids);
void 			initQueue(Queue* q, S_COImessage* data_arr, uint8_t size);
void			enqueue(Queue* q, S_COImessage* element);
void 			dequeue(Queue* q, S_COImessage* msg);
bool 			isQueueEmpty(Queue* q);
bool 			isQueueFull(Queue* q);



#endif /* INC_CAN_DS_H_ */
