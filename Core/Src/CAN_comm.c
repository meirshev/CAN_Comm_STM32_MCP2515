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
 * @brief Attempts to send the next in-queue CAN message if possible.
 *
 * This function first checks the queue of CAN messages to send and tries
 * to send the next message in the queue. If the queue is empty, it tries
 * to send the current message directly.
 *
 * @param hndl: a handle to a CAN communication module.
 * @param msg: message to send over the CAN module.
 *
 * */
void sendDataManager(CAN_HNDL hndl, S_COImessage* msg)
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
}

/**
 * @brief Attempts to send a CAN message and receive incoming CAN messages.
 *
 * This function first tries to send the next message in the send messages
 * queue. Then it tries to read a pending message from the MCP1515 module.
 *
 * @param hndl: a handle to a CAN communication module.
 *
 * */
void loop(CAN_HNDL hndl)
{
	S_COImessage sendMsg;

	if (dequeue(&hndl->sendQ, &sendMsg))
	{
		_sendCANMsg(hndl, &sendMsg);
	}

	_recvCANMsg();
}

/**
 * @brief reads the next CAN message from the received messages queue.
 *
 * This function reads the next CAN message from the received messages
 * queue.
 *
 * @param hndl: a handle to a CAN communication module.
 * @param msg: a pointer to a S_COImessage to place the dequeued message.
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

/**
 * @brief Configures the local CAN node parameters.
 *
 * This function configures the CAN node communication parameters -
 * filters, IDs, masks.
 *
 * @param hndl: a handle to a CAN communication module.
 * @param id: this is set according to the 'CAN_ds.h' module's 'NODE_IDS'
 * enumerator, please refer to the 'CAN_ds.h' module for more info.
 * @param commList: a boolean array where each index corresponds to
 * a node in the CAN communication network (of size 'NUM_OF_NODES', see
 * 'CAN_ds.h' module for more details), 'true' states that this node
 * should listen to the node corresponding to that index.
 * @param listenToAll: if set to true, this node will listen to all the
 * nodes that exists in the CAN network.
 *
 * @return 1 if the MCP2515 CAN module was successfully initialized, 0 otherwise. */
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

/**
 * @brief this function sends a given S_COImessage over the
 * CAN network.
 *
 * This function takes a S_COImessage, encodes it
 * into CAN based, MCP1515 CAN module format and then sends
 * it to the CAN network's nodes.
 *
 * @param hndl: a handle to a CAN communication module.
 * @param msg: a pointer to a S_COImessage to send.
 *
 **/
void _sendCANMsg(CAN_HNDL hndl, S_COImessage* msg)
{
	uCAN_MSG cMsg_1;
	uCAN_MSG cMsg_2;

	_encodeCANMsg(hndl, msg, &cMsg_1, &cMsg_2);
	CANSPI_Transmit(&cMsg_1);
	CANSPI_Transmit(&cMsg_2);
}

/**
 * @brief this function checks for a incoming messages and
 * stores them.
 *
 * This function checks the MCP2515 CAN module for pending
 * CAN messages, in case there's a pending message, it is
 * read and checked if its other part is already stored, if
 * so, the two CAN messages are combined to construct a full
 * S_COImessage and then store it in the received messages queue.
 *
 **/
void _recvCANMsg()
{
	uCAN_MSG rxMsg;

	if(not CANSPI_Receive(&rxMsg))
	{
		return;
	}
	else
	{
		_CANMsgHandler(&rxMsg);
	}
}

/**
 * @brief this function encodes an S_COImessage into a valid
 * CAN message to be sent over the network
 *
 * This function decomposes a S_COImessage into two CAN
 * messages, the first CAN message stores the timestamp
 * and the inertia and the second CAN message stores the
 * timestamp and omega. The inclusion of the timestamp data
 * in both CAN messages enables the decoding of the two
 * messages in the receiving CAN node.
 *
 * @param hndl: a handle to a CAN communication module.
 * @param msg: a pointer to a S_COImessage to encode.
 * @param cMsg_1: a pointer to a uCAN_MSG to store the first part of the
 * S_COImessage.
 * @param cMsg_2: a pointer to a uCAN_MSG to store the second part of the
 * S_COImessage.
 *
 **/
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

/**
 * @brief this function decodes a two CAN message into an
 * S_COImessage.
 *
 * This function takes two received CAN messages and extracts
 * the timestamp, J (inertia) and omega to form an S_COImessage.
 *
 * @param CANMsgPart1: a pointer to a uCAN_MSG that stores the first
 * part of the S_COImessage.
 * @param CANMsgPart2: a pointer to a uCAN_MSG that stores the second
 * part of the S_COImessage.
 * @param decodedMsg: a pointer to an S_COImessage to store the decoded
 * message.
 *
 **/
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

/**
 * @brief this function decodes and stores CAN messages.
 *
 * This function checks if the newly received message
 * has its complement message already stored, if so, it will
 * decode the message and construct a corresponding S_COImessage
 * to store in the received messages queue. else it will immediately
 * store this message in the waiting list.
 *
 * @param cMsg: a newly received CAN message.
 *
 * */
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
		enqueue(&_cData.recvQ, &msg);
	}
	else
	{
		/* replace old CAN message with the new one */
		_storeCANMsg(cMsg, sendingNodeID);
	}
}

/**
 * @brief this function stores a received CAN message.
 *
 * This function stores a received CAN message in the
 * waiting list for incomplete messages (can't construct
 * an S_COImessage, 1 part is missing).
 *
 * @param cMsg: a newly received CAN message.
 * @param msgIdx: the corresponding message index. this value
 * is actually the CAN node's index in the CAN network.
 *
 * */
void _storeCANMsg(uCAN_MSG *cMsg, int msgIdx)
{
	for(int i=0; i<CAN_MSG_SIZE; i++)
	{
		msgsBuffer[msgIdx].array[i] = cMsg->array[i];
	}
}
