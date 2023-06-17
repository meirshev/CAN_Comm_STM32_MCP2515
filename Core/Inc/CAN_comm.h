/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CAN_comm.h
  * @brief          : CAN communication User API.
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

#define MSG_PART_1(id) (0x00 | id)
#define MSG_PART_2(id) (0x80 | id)

#define ID_MASK  	0x7F
#define PART_MASK	0X80

typedef int bool;

/*
 * Contains all the relevant data for supporting the
 * app CAN communication requirements.
 * */
typedef struct{
	Queue 		sendQ;
	Queue 		recvQ;
	CommConfig 	idsList;
	uint8_t 	selfID;
}CAN_data;

/* a buffer usage data structure used for the conversion
 * of the encoded data into a timestamp value
 * */
typedef union
{
	struct{
		uint32_t 	timeStamp;
	}param;

	uint8_t 	_data[4];
}timeDecoder;

typedef CAN_data* CAN_HNDL;

/* static data structures used by the CAN communication module */
CAN_data 		_cData;
CAN_filters		_filters;
S_COImessage	sendMsgsArr[SEND_QUEUE_SIZE];
S_COImessage	recvMsgsArr[RECEIVE_QUEUE_SIZE];
uCAN_MSG		msgsBuffer[NUM_OF_NODES];


/* Public Functions Prototypes */
CAN_HNDL	initCANComm();
void 		loop(CAN_HNDL hndl);
void 		sendDataManager(CAN_HNDL hndl, S_COImessage* msg);
int 		readNextMsg(CAN_HNDL hndl, S_COImessage* msg);
uint8_t		configID(CAN_HNDL hndl, uint8_t id, CommConfig* commList, bool listenToAll);

/* Private Functions Prototypes */
void 		_sendCANMsg(CAN_HNDL hndl, S_COImessage* msg);
void 		_recvCANMsg();
void 		_encodeCANMsg(CAN_HNDL hndl, S_COImessage* msg, uCAN_MSG *cMsg_1, uCAN_MSG *cMsg_2);
void 		_decodeCANMsg(uCAN_MSG *CANMsgPart1, uCAN_MSG *CANMsgPart2, S_COImessage* decodedMsg);
void 		_CANMsgHandler(uCAN_MSG *cMsg);
void		_storeCANMsg(uCAN_MSG *cMsg, int msgIdx);

#endif /* INC_CAN_COMM_H_ */
