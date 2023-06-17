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

#define NUM_OF_NODES 6

#define true 	1
#define false 	0

typedef int bool;

/* every node in the CAN network must be assigned
 * with one of the IDs defined in this enumerator.
 * furthermore, no two CAN nodes in the network can
 * be assigned with the same ID.
 *  */
enum NODE_IDS{

	NODE_O_ID 	= 0x01,		// ID: 00000001
	NODE_1_ID 	= 0x02,		// ID: 00000010
	NODE_2_ID 	= 0x04,		// ID: 00000100
	NODE_3_ID 	= 0x08,		// ID: 00001000
	NODE_4_ID 	= 0x10,		// ID: 00010000
	NODE_5_ID 	= 0x20,		// ID: 00100000
	NODE_6_ID 	= 0x40,		// ID: 01000000

	NO_ID 		= -1		// For safety check-ups - need to implement in the future.
};

/* S_COImessage data structure, contains all the relevant
 * data to perform the center of inertia algorithms. */
typedef union{

	struct{
		uint32_t 	timeStamp;
		float 		J;
		float		omega;
	}params;
	uint8_t _data[12];

}S_COImessage;

/* the CommConfig data structure contains an array of all
 * the nodes in the CAN network where each index in the
 * array corresponds to a node's number in the network.
 * each element in the corresponding index states whether
 * this node is listening to any of the other nodes by
 * a simple true or false value.
 * */
typedef struct{

	bool commNodes[NUM_OF_NODES];

}CommConfig;

/* a queue data structure, its corresponding methods
 * for adding and getting elements are implemented below.
 * */
typedef struct{
	S_COImessage 	*msgsArr;
	uint8_t		 	QSize;
    int 			front;
    int 			rear;
} Queue;

/* functions prototypes */
void 			initCommConfig(CommConfig* nodes, uint8_t* selfID);
int				getNodeIndex(int nodeID);

void 			initQueue(Queue* q, S_COImessage* msgsArr, uint8_t size);
void			enqueue(Queue* q, S_COImessage* element);
int 			dequeue(Queue* q, S_COImessage* msg);
bool 			isQueueEmpty(Queue* q);
bool 			isQueueFull(Queue* q);


#endif /* INC_CAN_DS_H_ */
