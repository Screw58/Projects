/*
 * main.h
 *
 *  Created on: Jun 8, 2022
 *      Author: Jacek
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_


#include "stm32f4xx_hal.h"

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

void Error_Handler(void);
void SystemClock_Config(void);

#endif /* INC_MAIN_H_ */
