/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************/

#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H


void SysTick_Handler(void);
void DMA1_Stream6_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

#endif /* __STM32F4xx_IT_H */
