/**
  ******************************************************************************
  * @file    motors.h
  * @brief   This file provide defines gpios and function declaration
  ******************************************************************************/

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_



#include "stm32f4xx_hal.h"

/*
 *  @attention Enable1 pin and Enable2 pin of the L293D H bridge,
 *  			are connected to PA6 (timer3 output PWM )
 */

#define M1A GPIO_PIN_12
#define M2A GPIO_PIN_13
#define M1B GPIO_PIN_14
#define M2B GPIO_PIN_15
#define MOTORS_PORT GPIOB

void go_straight(void);
void turn_left(void);
void turn_right(void);
void stop_move(void);

#endif /* INC_MOTORS_H_ */
