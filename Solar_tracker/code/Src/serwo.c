/**
  ******************************************************************************
  * @file    serwo.h
  * @author  Jacek
  * @brief   This file provides stepper motor initialize function, functions for
  * 		 handling and setting parameters of the motor
  ******************************************************************************/

#include "main.h"
#include "serwo.h"


void Serwo_set_speed(serwo_t *serwo, uint8_t speed)
{
	if(speed >= SERWO_SPEED_1 && speed <= SERWO_SPEED_6)
	serwo->speed = speed ;
}


void Serwo_set_pos(serwo_t *serwo, uint8_t position)
{
	if( position >= SERWO_MIN_POS && position <= SERWO_MAX_POS)
	serwo->given_position = position;
}

void Serwo_set_mode(serwo_t *serwo, serwo_mode_t mode)
{
	if(mode == directly || mode==indirectly)
	serwo->mode = mode ;
}

void Serwo_Init(serwo_t *serwo, TIM_HandleTypeDef * timer, uint32_t channel )
{
	serwo->timer = timer;
	serwo->channel = channel;
}




void Serwo_start(serwo_t *serwo)
{
	if(serwo->mode == directly)
	{
		serwo->timer->Instance->CCR1 = 80000U + (serwo->given_position * PULSE_ON_DEGREE);
	}
	else
	{
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		serwo->counter_given = 50/serwo->speed ;
	}
HAL_NVIC_DisableIRQ(TIM2_IRQn);

}


void Serwo_stop(serwo_t *serwo)
{

	serwo->given_position = serwo->actual_position ;
	//disable irq
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
}




void Serwo_interrupt(serwo_t *serwo)
{
	if(serwo->counter == serwo->counter_given)
	{
		if(serwo->actual_position < serwo->given_position)
		{
			serwo->timer->Instance->CCR1 +=  PULSE_ON_DEGREE;
			serwo->actual_position += 1;
		}
		else if(serwo->actual_position > serwo->given_position)
		{
			serwo->timer->Instance->CCR1 -=  PULSE_ON_DEGREE;
			serwo->actual_position -= 1;
		}
		else
		{
			//!< TODO give = actual -- disable irq
		}

	serwo->counter = 0;

	}
	else
	{
		serwo->counter += 1 ;
	}

}







