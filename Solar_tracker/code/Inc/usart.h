/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************/

#ifndef __USART_H__
#define __USART_H__



#include "main.h"


extern UART_HandleTypeDef huart2;

void MX_USART2_UART_Init(void);

#endif /* __USART_H__ */

