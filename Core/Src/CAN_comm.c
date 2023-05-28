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
	initCommConfig(&_cData.idsList, &selfID);

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

bool configID(CAN_HNDL hndl, uint8_t id, CommConfig* commList, bool listenToAll)
{

	uint8_t mask 	= 0x00;

	if (!listenToAll)
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
		return true;
	}
	else
	{
		return false;
	}

}

void _addToRecvQ(CAN_HNDL hndl, S_COImessage* msg)
{

}

void _convertToCANFormat(S_COImessage* msg)
{

}

void 	_sendCANMsg(S_COImessage* msg)
{

}
