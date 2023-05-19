#ifndef __CAN_SPI_H
#define	__CAN_SPI_H

#include "stm32h7xx_hal.h"

#define true  1
#define false 0
typedef int bool;


typedef union {

	struct {
		uint8_t		idType;			// 1 Bytes.
		uint32_t 	id;				// 4 Bytes.
		uint8_t 	dlc;			// 1 Bytes.
		float 		inertia;		// 4 Bytes.
		float		frequency;		// 4 Bytes.
		double 		timeStamp;		// 8 Bytes.
	}frame;

	uint8_t	array[22];

}S_COImessage;


typedef union {
  struct {
    uint8_t idType;
    uint32_t id;
    uint8_t dlc;
    uint8_t data0;
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
    uint8_t data4;
    uint8_t data5;
    uint8_t data6;
    uint8_t data7;
  } frame;
  uint8_t array[14];
} uCAN_MSG;

typedef union {
  struct {
    uint8_t idType;
    uint32_t id;
    uint8_t dlc;
    uint8_t data0;
    uint8_t data1;
  } frame;
  uint8_t array[14];	// maybe change that accordingly.
} uCAN_MSG_2B;

#define dSTANDARD_CAN_MSG_ID_2_0B 1
#define dEXTENDED_CAN_MSG_ID_2_0B 2

bool CANSPI_Initialize(void);
void CANSPI_Sleep(void);
uint8_t CANSPI_Transmit(uCAN_MSG *tempCanMsg);
uint8_t CANSPI_Transmit_2B(uCAN_MSG_2B *tempCanMsg);
uint8_t CANSPI_Receive(uCAN_MSG *tempCanMsg);
uint8_t CANSPI_Receive_2B(uCAN_MSG_2B *tempCanMsg);
uint8_t CANSPI_messagesInBuffer(void);
uint8_t CANSPI_isBussOff(void);
uint8_t CANSPI_isRxErrorPassive(void);
uint8_t CANSPI_isTxErrorPassive(void);

#endif	/* __CAN_SPI_H */
