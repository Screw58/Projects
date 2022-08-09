  /******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  */

#ifndef __USART_H__
#define __USART_H__




#include "main.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


void USART2_Init(void);
void USART3_Init(void);



#endif /* __USART_H__ */

