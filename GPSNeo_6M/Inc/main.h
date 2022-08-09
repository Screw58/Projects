
 /******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************/

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"


#define LED_Pin 	GPIO_PIN_5
#define LED_Port 	GPIOA

#define INCOMPLETE_INFO		0
#define COMPLETE_INFO		1
#define NOT_SENDING			0
#define SENDING				1
#define CIRC_BUFF_SIZE 			250
#define WORK_BUF_SIZE 			85

typedef struct
{
	char * const buffer;
	volatile uint8_t head;
	volatile uint8_t tail;

}circ_buff_t;




void Error_Handler(void);
void System_clock_config(void);
void display_info(void);
#endif /* __MAIN_H */
