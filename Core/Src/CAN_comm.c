/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CAN_comm.c
  * @brief          : CAN communication api.
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

#include "CAN_comm.h"


CAN_HNDL initCANComm()
{
	initQueue(&_cData.sendQ, sendQ, SEND_QUEUE_SIZE);
	initQueue(&_cData.recvQ, recvQ, RECEIVE_QUEUE_SIZE);
	initIDsList(&_cData.idsList);

	return &_cData;
}


int sendDataManager(CAN_HNDL hndl, S_COImessage* msg)
{
	S_COImessage* msgToSend;

	if (isQueueEmpty(&hndl->sendQ))
	{
		msgToSend = msg;
	}
	else
	{
		dequeue(&hndl->sendQ, msgToSend);
		enqueue(&hndl->sendQ, msg);
	}

	_sendCANMsg(msgToSend);
}

void loop()
{

}

int readNextMsg(CAN_HNDL hndl, S_COImessage* msg)
{
	if (isQueueEmpty(&hndl->recvQ))
	{
		return 0;
	}

	dequeue(&hndl->sendQ, msg);

	return 1;
}

int	configID(CAN_HNDL hndl, uint8_t id, IDs* idsList, bool listenToAll)
{
	return 1;
}

void _addToRecvQ(CAN_HNDL hndl, S_COImessage* msg)
{

}

void _convertToCANFormat(S_COImessage* msg)
{

}

void _sendCANMsg(S_COImessage* msg)
{

}
