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

/**
 * @brief this function initializes the CAN communication module
 * configuration.
 *
 * @param nodes: an array of all the nodes in the CAN network,
 * each node in the network is numbered and corresponds to an
 * index in the array where the stored value in that index
 * (true or false) means whether this node listens on incoming
 * CAN messages from that node.
 * @param selfID: the node ID in the CAN network of this node.
 *
 * */
void initCommConfig(CommConfig* nodes, uint8_t* selfID)
{
	*selfID = NO_ID;

	for (int i=0; i < NUM_OF_NODES; i++)
	{
		nodes->commNodes[i] = false;
	}
}

/**
 * @brief this function initializes a queue data structure.
 *
 * @param q: the q to be initialized.
 * @param msgsArr: reserved memory for the queue.
 * @param size: size of queue, should correspond to the size
 * of msgsArr.
 *
 * */
void initQueue(Queue* q, S_COImessage* msgsArr, uint8_t size)
{
	q->QSize 	= size;
	q->msgsArr 	= msgsArr;
    q->front 	= -1;
    q->rear 	= -1;
}

/**
 * @brief this function checks whether a given queue is
 * empty or not.
 *
 * @param q: the queue to be checked.
 *
 * @return: true if the queue is empty, false otherwise.
 * */
bool isQueueEmpty(Queue* q)
{
    return (q->front == -1 && q->rear == -1);
}

/**
 * @brief this function checks whether a given queue
 * is full or not.
 *
 * @param q: the queue to be checked
 *
 * @return: true if the queue is empty, false otherwise.
 *
 * */
bool isQueueFull(Queue* q)
{
    return ((q->rear + 1) % q->QSize == q->front);
}

/**
 * @Brief adds an element to the queue.
 *
 * @param q: the queue of interest.
 * @param element: the element to be added to the queue.
 *
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

/**
 * @brief this function gets the next element in the queue.
 *
 * @param q: the queue of interest.
 * @param msg: the dequeued message will be stored in the
 * memory storage pointed by this pointer.
 *
 * @return: 1 if the operation was successfull, 0 if the
 * queue is empty.
 * */
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

/**
 * @brief this function checks a given node's ID and
 * returns its corresponding CAN network number.
 *
 * @param nodeID: the ID of the node one wishes to check its
 * index.
 *
 * @return: the index of the corresponding node ID.
 *
 * */
int getNodeIndex(int nodeID)
{
	if (nodeID == NODE_O_ID)
	{
		return 0;
	}
	else if(nodeID == NODE_1_ID)
	{
		return 1;
	}
	else if(nodeID == NODE_2_ID)
	{
		return 2;
	}
	else if(nodeID == NODE_3_ID)
	{
		return 3;
	}
	else if(nodeID == NODE_4_ID)
	{
		return 4;
	}
	else if(nodeID == NODE_5_ID)
	{
		return 5;
	}
	else if(nodeID == NODE_6_ID)
	{
		return 6;
	}
	else
	{
		return -1;
	}
}
