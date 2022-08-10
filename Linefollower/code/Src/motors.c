/**
  ******************************************************************************
  * @file    motors.c
  * @brief   This file contains functions definitions which control GPIOs
  * 		 to the handling H bridge pins. Speed is controlled by TIM3.
  ******************************************************************************/

#include "motors.h"

void go_straight(void)
{
	HAL_GPIO_WritePin(MOTORS_PORT, M1A, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M1B, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2B, GPIO_PIN_SET);
}

void turn_left(void)
{
	HAL_GPIO_WritePin(MOTORS_PORT, M1A, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M1B, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2B, GPIO_PIN_RESET);
}

void turn_right(void)
{
	HAL_GPIO_WritePin(MOTORS_PORT, M1A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M1B, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2B, GPIO_PIN_SET);
}

void stop_move(void)
{
	HAL_GPIO_WritePin(MOTORS_PORT, M1A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M1B, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORS_PORT, M2B, GPIO_PIN_RESET);
}
