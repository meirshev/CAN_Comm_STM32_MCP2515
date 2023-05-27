/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : debug_tools.c
  * @brief          : Simple debug tools, UART based.
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


#include <debug_tools.h>

uint8_t uart_rx_buff_1[1];
char 	msg_buff[50];
int		msg_buff_len;

void printToConsole(UART_HandleTypeDef *huart, char msg[50])
{
	msg_buff_len = sprintf(msg_buff, msg);
	HAL_UART_Transmit(huart, (uint8_t *)msg_buff, msg_buff_len, 100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(huart, uart_rx_buff_1, 1);
}


void block(UART_HandleTypeDef *huart, int user_input)
{
	uart_rx_buff_1[0] = 0;
	HAL_UART_Receive_IT(huart, uart_rx_buff_1, 1);
	while((uart_rx_buff_1[0] - 48) != user_input)
	{
		HAL_Delay(200);
	}
}
