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
	initQueue(&_cData.sendQ, sendMsgsArr, SEND_QUEUE_SIZE);
	initQueue(&_cData.recvQ, recvMsgsArr, RECEIVE_QUEUE_SIZE);
	initCommConfig(&_cData.idsList, &selfID);

	return &_cData;
}


int sendDataManager(CAN_HNDL hndl, S_COImessage* msg)
{

	if (isQueueEmpty(&hndl->sendQ))
	{
		_sendCANMsg(msg);
	}
	else
	{
		S_COImessage buffMsg;
		dequeue(&hndl->sendQ, &buffMsg);
		enqueue(&hndl->sendQ, msg);
		_sendCANMsg(msg);
	}

	return 1;		// not yet fully configured - need to decided what to do with the return value of CANSPI_Transmit() func.
}

void loop(CAN_HNDL hndl)
{

	uCAN_MSG rxMsg;

	if(CANSPI_Receive(&rxMsg))
	{
		S_COImessage recvMsg;
		_decodeCANMsg(&rxMsg, &recvMsg);
		enqueue(&hndl->recvQ, &recvMsg);
	}

	S_COImessage sendMsg;

	if (dequeue(&hndl->sendQ, &sendMsg))
	{
		uCAN_MSG cMsg_1;
		uCAN_MSG cMsg_2;

		_encodeCANMsg(&sendMsg, &cMsg_1, &cMsg_2);
		CANSPI_Transmit(&cMsg_1);
		CANSPI_Transmit(&cMsg_2);
	}
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
	filters.RXM0reg.RXM0SIDH = mask >> 3;
	filters.RXM0reg.RXM0SIDL = mask << 5;
	filters.RXM0reg.RXM0EID8 = 0x00;
	filters.RXM0reg.RXM0EID0 = 0x00;

	filters.RXM1reg.RXM1SIDH = 0x00;
	filters.RXM1reg.RXM1SIDL = 0x00;
	filters.RXM1reg.RXM1EID8 = 0x00;
	filters.RXM1reg.RXM1EID0 = 0x00;

	/* Rx filter values */
	filters.RXF0reg.RXF0SIDH = 0x00;
	filters.RXF0reg.RXF0SIDL = 0x00;
	filters.RXF0reg.RXF0EID8 = 0x00;
	filters.RXF0reg.RXF0EID0 = 0x00;

	filters.RXF1reg.RXF1SIDH = 0x00;
	filters.RXF1reg.RXF1SIDL = 0x00;
	filters.RXF1reg.RXF1EID8 = 0x00;
	filters.RXF1reg.RXF1EID0 = 0x00;

	filters.RXF2reg.RXF2SIDH = 0x00;
	filters.RXF2reg.RXF2SIDL = 0x00;
	filters.RXF2reg.RXF2EID8 = 0x00;
	filters.RXF2reg.RXF2EID0 = 0x00;

	filters.RXF3reg.RXF3SIDH = 0x00;
	filters.RXF3reg.RXF3SIDL = 0x00;
	filters.RXF3reg.RXF3EID8 = 0x00;
	filters.RXF3reg.RXF3EID0 = 0x00;

	filters.RXF4reg.RXF4SIDH = 0x00;
	filters.RXF4reg.RXF4SIDL = 0x00;
	filters.RXF4reg.RXF4EID8 = 0x00;
	filters.RXF4reg.RXF4EID0 = 0x00;

	filters.RXF5reg.RXF5SIDH = 0x00;
	filters.RXF5reg.RXF5SIDL = 0x00;
	filters.RXF5reg.RXF5EID8 = 0x00;
	filters.RXF5reg.RXF5EID0 = 0x00;


	if (CANSPI_Initialize(&filters))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

void _sendCANMsg(S_COImessage* msg)
{

//	uCAN_MSG sendMsg_1;
//	uCAN_MSG sendMsg_2;


//	CANSPI_Transmit();
}

void _recvCANMsg(S_COImessage* msg)
{

}

void _encodeCANMsg(S_COImessage* msg, uCAN_MSG *cMsg_1, uCAN_MSG *cMsg_2)
{
	cMsg_1->frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
	cMsg_1->frame.id = selfID;
	cMsg_1->frame.dlc = 8;				// message size - 8 bytes.

	cMsg_1->frame.data0 = msg->_data[0];	// timestamp param
	cMsg_1->frame.data1 = msg->_data[1];	// timestamp param
	cMsg_1->frame.data2 = msg->_data[2];	// timestamp param
	cMsg_1->frame.data3 = msg->_data[3];	// timestamp param

	cMsg_1->frame.data4 = msg->_data[4];	// J param
	cMsg_1->frame.data5 = msg->_data[5];	// J param
	cMsg_1->frame.data6 = msg->_data[6];	// J param
	cMsg_1->frame.data7 = msg->_data[7];	// J param

	cMsg_2->frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
	cMsg_2->frame.id = selfID;
	cMsg_2->frame.dlc = 8;				// message size - 8 bytes.

	cMsg_2->frame.data0 = msg->_data[0];	// timestamp param
	cMsg_2->frame.data1 = msg->_data[1];	// timestamp param
	cMsg_2->frame.data2 = msg->_data[2];	// timestamp param
	cMsg_2->frame.data3 = msg->_data[3];	// timestamp param

	cMsg_2->frame.data4 = msg->_data[4];	// omega param
	cMsg_2->frame.data5 = msg->_data[5];	// omega param
	cMsg_2->frame.data6 = msg->_data[6];	// omega param
	cMsg_2->frame.data7 = msg->_data[7];	// omega param
}

void _decodeCANMsg(uCAN_MSG *cMsg, S_COImessage* msg)
{

}
