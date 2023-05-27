/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : debug_tools.h
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

#ifndef __BLOCKER
#define __BLOCKER

#include "stm32h7xx_hal.h"
#include <stdio.h>

void printToConsole(UART_HandleTypeDef *huart, char msg[50]);
void block(UART_HandleTypeDef *huart, int user_input);

#endif
