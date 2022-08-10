  /******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************/

#ifndef __USART_H__
#define __USART_H__


#include "main.h"


#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA

extern UART_HandleTypeDef huart2;


void USART2_UART_Init(void);

#endif /* __USART_H__ */

