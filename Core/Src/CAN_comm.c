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

/**
 * @brief Initializes the CAN communication module.
 *
 * This function initializes the CAN communication module by initializing
 * the send and receive queues, as well as the communication configuration
 * including the ID list and the self ID.
 *
 * @return A pointer to a CAN communication handle.
 */
CAN_HNDL initCANComm()
{
	initQueue(&_cData.sendQ, sendMsgsArr, SEND_QUEUE_SIZE);
	initQueue(&_cData.recvQ, recvMsgsArr, RECEIVE_QUEUE_SIZE);
	initCommConfig(&_cData.idsList, &_cData.selfID);

	return &_cData;
}

/**
 * @brief Attempts to send the next in queue CAN message if possible.
 *
 * This function first checks the queue of CAN messages to send and tries
 * to send the next message in the queue. If the queue is empty, it tries
 * to send the current message directly.
 *
 *
 * @return 1 if a message was sent, 0 otherwise. */
int sendDataManager(CAN_HNDL hndl, S_COImessage* msg)
{

	if (isQueueEmpty(&hndl->sendQ))
	{
		_sendCANMsg(hndl, msg);
	}
	else
	{
		S_COImessage buffMsg;
		dequeue(&hndl->sendQ, &buffMsg);
		enqueue(&hndl->sendQ, msg);
		_sendCANMsg(hndl, msg);
	}

	return 1;		// not yet fully configured - need to decide what to do with the return value of CANSPI_Transmit() func.
}

/**
 * @brief Attempts to send a CAN message and receive incoming CAN messages.
 *
 * This function first tries to send the next message in the send messages
 * queue. Then it tries to read a pending message from the MCP1515 module.
 *
 * */
void loop(CAN_HNDL hndl)
{
	S_COImessage sendMsg;

	if (dequeue(&hndl->sendQ, &sendMsg))
	{
		_sendCANMsg(hndl, &sendMsg);
	}

}

/**
 * @brief reads the next CAN message from the received messages queue.
 *
 * This function reads the next CAN message from the received messages
 * queue.
 *
 * @return 1 if a message was read, 0 if the queue is empty. */
int readNextMsg(CAN_HNDL hndl, S_COImessage* msg)
{
	if (isQueueEmpty(&hndl->recvQ))
	{
		return 0;
	}

	dequeue(&hndl->sendQ, msg);

	return 1;
}

uint8_t configID(CAN_HNDL hndl, uint8_t id, CommConfig* commList, bool listenToAll)
{

	uint8_t mask = 0x00;

	if (not listenToAll)
	{
		for(int id_x=0; id_x<NUM_OF_NODES; id_x++)
		{
			if ((commList->commNodes[id_x] == false) & (id_x == 0))
			{
				mask = mask | NODE_O_ID;
			}
			else if((commList->commNodes[id_x] == false) & (id_x == 1))
			{
				mask = mask | NODE_1_ID;
			}
			else if((commList->commNodes[id_x] == false) & (id_x == 2))
			{
				mask = mask | NODE_2_ID;
			}
			else if((commList->commNodes[id_x] == false) & (id_x == 3))
			{
				mask = mask | NODE_3_ID;
			}
			else if((commList->commNodes[id_x] == false) & (id_x == 4))
			{
				mask = mask | NODE_4_ID;
			}
			else if((commList->commNodes[id_x] == false) & (id_x == 5))
			{
				mask = mask | NODE_5_ID;
			}
			else if((commList->commNodes[id_x] == false) & (id_x == 6))
			{
				mask = mask | NODE_6_ID;
			}
		}
	}

	/* Rx masks values*/
	_filters.RXM0reg.RXM0SIDH = mask >> 3;
	_filters.RXM0reg.RXM0SIDL = mask << 5;
	_filters.RXM0reg.RXM0EID8 = 0x00;
	_filters.RXM0reg.RXM0EID0 = 0x00;

	_filters.RXM1reg.RXM1SIDH = 0x00;
	_filters.RXM1reg.RXM1SIDL = 0x00;
	_filters.RXM1reg.RXM1EID8 = 0x00;
	_filters.RXM1reg.RXM1EID0 = 0x00;

	/* Rx filter values */
	_filters.RXF0reg.RXF0SIDH = 0x00;
	_filters.RXF0reg.RXF0SIDL = 0x00;
	_filters.RXF0reg.RXF0EID8 = 0x00;
	_filters.RXF0reg.RXF0EID0 = 0x00;

	_filters.RXF1reg.RXF1SIDH = 0x00;
	_filters.RXF1reg.RXF1SIDL = 0x00;
	_filters.RXF1reg.RXF1EID8 = 0x00;
	_filters.RXF1reg.RXF1EID0 = 0x00;

	_filters.RXF2reg.RXF2SIDH = 0x00;
	_filters.RXF2reg.RXF2SIDL = 0x00;
	_filters.RXF2reg.RXF2EID8 = 0x00;
	_filters.RXF2reg.RXF2EID0 = 0x00;

	_filters.RXF3reg.RXF3SIDH = 0x00;
	_filters.RXF3reg.RXF3SIDL = 0x00;
	_filters.RXF3reg.RXF3EID8 = 0x00;
	_filters.RXF3reg.RXF3EID0 = 0x00;

	_filters.RXF4reg.RXF4SIDH = 0x00;
	_filters.RXF4reg.RXF4SIDL = 0x00;
	_filters.RXF4reg.RXF4EID8 = 0x00;
	_filters.RXF4reg.RXF4EID0 = 0x00;

	_filters.RXF5reg.RXF5SIDH = 0x00;
	_filters.RXF5reg.RXF5SIDL = 0x00;
	_filters.RXF5reg.RXF5EID8 = 0x00;
	_filters.RXF5reg.RXF5EID0 = 0x00;


	if (CANSPI_Initialize(&_filters))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * This function currently is not checking whether the transmission
 * operation was indeed successful.
 * */
void _sendCANMsg(CAN_HNDL hndl, S_COImessage* msg)
{
	uCAN_MSG cMsg_1;
	uCAN_MSG cMsg_2;

	_encodeCANMsg(hndl, msg, &cMsg_1, &cMsg_2);
	CANSPI_Transmit(&cMsg_1);
	CANSPI_Transmit(&cMsg_2);
}

void _recvCANMsg(CAN_HNDL hndl, uCAN_MSG *CANMsg)
{

	uCAN_MSG rxMsg;

	if(not CANSPI_Receive(&rxMsg))
	{
		return;
	}


//	_decodeCANMsg(CANMsg, &recvMsg);
//	enqueue(&hndl->recvQ, &recvMsg);
}

void _encodeCANMsg(CAN_HNDL hndl, S_COImessage* msg, uCAN_MSG *cMsg_1, uCAN_MSG *cMsg_2)
{

	cMsg_1->frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
	cMsg_1->frame.id = MSG_PART_1(hndl->selfID);
	cMsg_1->frame.dlc = 8;					// message size - 8 bytes.

	cMsg_1->frame.data0 = msg->_data[0];	// timestamp param
	cMsg_1->frame.data1 = msg->_data[1];	// timestamp param
	cMsg_1->frame.data2 = msg->_data[2];	// timestamp param
	cMsg_1->frame.data3 = msg->_data[3];	// timestamp param

	cMsg_1->frame.data4 = msg->_data[4];	// J param
	cMsg_1->frame.data5 = msg->_data[5];	// J param
	cMsg_1->frame.data6 = msg->_data[6];	// J param
	cMsg_1->frame.data7 = msg->_data[7];	// J param

	cMsg_2->frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
	cMsg_2->frame.id = MSG_PART_2(hndl->selfID);
	cMsg_2->frame.dlc = 8;					// message size - 8 bytes.

	cMsg_2->frame.data0 = msg->_data[0];	// timestamp param
	cMsg_2->frame.data1 = msg->_data[1];	// timestamp param
	cMsg_2->frame.data2 = msg->_data[2];	// timestamp param
	cMsg_2->frame.data3 = msg->_data[3];	// timestamp param

	cMsg_2->frame.data4 = msg->_data[4];	// omega param
	cMsg_2->frame.data5 = msg->_data[5];	// omega param
	cMsg_2->frame.data6 = msg->_data[6];	// omega param
	cMsg_2->frame.data7 = msg->_data[7];	// omega param
}

void _decodeCANMsg(uCAN_MSG *CANMsgPart1, uCAN_MSG *CANMsgPart2, S_COImessage* decodedMsg)
{
	/* decode timestamp data */
	decodedMsg->_data[0] = CANMsgPart1->frame.data0;
	decodedMsg->_data[1] = CANMsgPart1->frame.data1;
	decodedMsg->_data[2] = CANMsgPart1->frame.data2;
	decodedMsg->_data[3] = CANMsgPart1->frame.data3;

	/* decode J (inertia) data */
	decodedMsg->_data[4] = CANMsgPart1->frame.data4;
	decodedMsg->_data[5] = CANMsgPart1->frame.data5;
	decodedMsg->_data[6] = CANMsgPart1->frame.data6;
	decodedMsg->_data[7] = CANMsgPart1->frame.data7;

	/* decode omega data */
	decodedMsg->_data[8]  = CANMsgPart2->frame.data4;
	decodedMsg->_data[9]  = CANMsgPart2->frame.data5;
	decodedMsg->_data[10] = CANMsgPart2->frame.data6;
	decodedMsg->_data[11] = CANMsgPart2->frame.data7;
}

void _CANMsgHandler(uCAN_MSG *cMsg)
{
	int sendingNodeID = getNodeIndex(cMsg->frame.id & ID_MASK);

	timeDecoder currentTime;
	timeDecoder storedTime;

	currentTime._data[0] = cMsg->frame.data0;
	currentTime._data[1] = cMsg->frame.data1;
	currentTime._data[2] = cMsg->frame.data2;
	currentTime._data[3] = cMsg->frame.data3;

	storedTime._data[0] = msgsBuffer[sendingNodeID].frame.data0;
	storedTime._data[1] = msgsBuffer[sendingNodeID].frame.data1;
	storedTime._data[2] = msgsBuffer[sendingNodeID].frame.data2;
	storedTime._data[3] = msgsBuffer[sendingNodeID].frame.data3;

	if (currentTime.param.timeStamp == storedTime.param.timeStamp)
	{
		S_COImessage 	msg;
		uCAN_MSG 		*CANMsgParts[2];

		CANMsgParts[(cMsg->frame.id & PART_MASK) >> 7] 						= cMsg;
		CANMsgParts[(msgsBuffer[sendingNodeID].frame.id & PART_MASK) >> 7] 	= &msgsBuffer[sendingNodeID];

		_decodeCANMsg(CANMsgParts[0], CANMsgParts[1], &msg);
		enqueue(&_cData.sendQ, &msg);
	}
	else
	{
		/* replace old CAN message with the new one */
		_storeCANMsg(cMsg, sendingNodeID);
	}
}

void _storeCANMsg(uCAN_MSG *cMsg, int msgIdx)
{
	for(int i=0; i<CAN_MSG_SIZE; i++)
	{
		msgsBuffer[msgIdx].array[i] = cMsg->array[i];
	}
}
