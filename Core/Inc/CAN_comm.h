/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CAN_comm.h
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

#ifndef INC_CAN_COMM_H_
#define INC_CAN_COMM_H_

#include "CAN_ds.h"
#include "CANSPI.h"
#include "MCP2515.h"
#include "stm32h7xx_hal.h"

#define SEND_QUEUE_SIZE 	1
#define RECEIVE_QUEUE_SIZE 	5

#define true 	1
#define false 	0
#define not 	!

typedef int bool;

typedef struct{
	Queue 		sendQ;
	Queue 		recvQ;
	CommConfig 	idsList;
}CAN_data;

typedef CAN_data* CAN_HNDL;

CAN_data 		_cData;
S_COImessage	sendMsgsArr[SEND_QUEUE_SIZE];
S_COImessage	recvMsgsArr[RECEIVE_QUEUE_SIZE];
CAN_filters		filters;
uint8_t 		selfID;		// should be set only from the NODE_IDS Enum.

/* Public Functions Prototypes */
CAN_HNDL	initCANComm();
void 		loop(CAN_HNDL hndl);
int 		sendDataManager(CAN_HNDL hndl, S_COImessage* msg);
int 		readNextMsg(CAN_HNDL hndl, S_COImessage* msg);
uint8_t		configID(CAN_HNDL hndl, uint8_t id, CommConfig* commList, bool listenToAll);

/* Private Functions Prototypes */
void 		_sendCANMsg(S_COImessage* msg);
void 		_recvCANMsg(S_COImessage* msg);
void 		_encodeCANMsg(S_COImessage* msg, uCAN_MSG *cMsg_1, uCAN_MSG *cMsg_2);
void 		_decodeCANMsg(uCAN_MSG *cMsg, S_COImessage* msg);
#endif /* INC_CAN_COMM_H_ */
