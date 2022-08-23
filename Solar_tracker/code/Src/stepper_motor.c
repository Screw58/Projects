/**
  ******************************************************************************
  * @file    stepper_motor.c
  * @author  Jacek
  * @brief   This file provides stepper motor initialize function, functions for
  * 		 handling and setting parameters of the motor using TMC2209 stepper
  * 		 motor driver
  ******************************************************************************/

#include "main.h"
#include "stepper_motor.h"




/**
  * @brief  Initializes the motor and binds the timer and channel which handling it
  * @note   Timer must be in PWM output mode
  * @param  stepper motor handle
  * @param	HAL tim base handle
  * @param 	TIM channel
  */
void Stepper_init(stepper_t * motor, TIM_HandleTypeDef * htim ,uint32_t channel)
{

	motor->timer = htim ;
	motor->channel = channel ;
	uint32_t temp = (HAL_RCC_GetPCLK1Freq() * 2) /  10000U ;
	motor->timer->Instance->PSC = temp - 1;

	if(motor->direction == right || motor->direction == left)
	{
		Stepper_set_dir(motor, motor->direction);
	}
	if(motor->speed > MIN_SPEED && motor->speed < MAX_SPEED )
	{
		Stepper_set_speed(motor, motor->speed);
	}
	if(motor->angle > 0)
	{
		Stepper_set_angle(motor, motor->angle);
	}
}


/**
  * @brief  Functions for set moving parameters of the motor
  */

void Stepper_set_angle(stepper_t * motor, uint16_t angle)
{
	motor->angle = angle;
	motor->angle_counter = (uint32_t)(angle / (ANGLE_PER_STEP / motor->micro_steps));
}

/**
  * @param  mode: off, continuous or angle
  */
void Stepper_set_mode(stepper_t * motor, stepper_mode_t mode )
{
	motor->mode = mode ;
}

/**
  * @param  speed must be value between 1 and 1000
  */
void Stepper_set_speed(stepper_t * motor, uint16_t speed)
{
	uint32_t temp=0;
	if(speed>MIN_SPEED && speed<=MAX_SPEED)
	{
	motor->speed = speed;
	temp = (uint32_t)(10000U / motor->speed);
	motor->timer->Instance->ARR = temp;
	temp = temp/2;
	motor->timer->Instance->CCR1 = (uint32_t)temp;

	}
}

/**
  * @param  dir: left or right
  */
void Stepper_set_dir(stepper_t * motor, dir_t direction )
{
	motor->direction = direction ;
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, motor->direction);

}


/**
  * @brief  Motor move starting
  * @note   This function should be use after setting all parameters for motor
  * @param  stepper motor handle
  */

void Stepper_start(stepper_t * motor)
{
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, motor->direction);

	if(motor->mode == continous )
	{
		HAL_TIM_PWM_Start(motor->timer, motor->channel);
	}

	else if(motor->mode == angle)
	{
		if(motor->angle_counter != 0)
		{
		HAL_TIM_PWM_Start_IT(motor->timer, motor->channel);

		}
	}
}

void Stepper_stop(stepper_t * motor)
{
	if(motor->mode == continous)
		HAL_TIM_PWM_Stop(motor->timer, motor->channel);
	else if(motor->mode == angle)
		HAL_TIM_PWM_Stop_IT(motor->timer, motor->channel);
}




/**
  * @brief  When angle mode is on, this function updates angle param
  * @note   This function must be paste in the HAL_TIM_PWM_PulseFinishedCallback
  * @param	HAL tim base handle
  * @param  stepper motor handle
  */
void Stepper_interrupt(TIM_HandleTypeDef * htim, stepper_t * motor)
{
	if(htim->Instance == motor->timer->Instance)
	{
		motor->angle_counter--;
		if(motor->angle_counter == 0)
		{
			Stepper_stop(motor);
		}
	}
}

/*
void stepper_off(void)
{
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
}

*/




