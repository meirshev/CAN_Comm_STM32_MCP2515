
#ifndef __BLOCKER
#define __BLOCKER

#include "stm32h7xx_hal.h"
#include <stdio.h>

void print_to_console(UART_HandleTypeDef *huart, char msg[50]);
void block(UART_HandleTypeDef *huart, int user_input);

#endif
