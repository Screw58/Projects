/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file contains timers functions definitions.
  * 		 TIM2 -> modulates signal for IR transmitting diodes
  * 		 TIM3 -> control speed of dc engines
  ******************************************************************************/

#include "tim.h"
#include "gpio.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;


extern volatile GPIO_Reg_user_t  *pGPIOA_user;
static uint32_t  *pRCC_APB1ENR_user = (uint32_t*)(0x40023800+0x40);



void TIM2_Init(void)
{
	TIM_OC_InitTypeDef tim2_config = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if(HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}

	tim2_config.Pulse = 200;
	tim2_config.OCMode = TIM_OCMODE_PWM1;
	tim2_config.OCPolarity = TIM_OCPOLARITY_HIGH;

	if(HAL_TIM_PWM_ConfigChannel(&htim2, &tim2_config, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
}



void TIM3_Init(void)
{
	TIM_OC_InitTypeDef  tim3_config={0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 100-1 ;
	htim3.Init.Period= 400-1 ;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	if(HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}

	tim3_config.Pulse = 250-1 ;
	tim3_config.OCPolarity = TIM_OCPOLARITY_HIGH ;
	tim3_config.OCMode =  TIM_OCMODE_PWM1;

	if (HAL_TIM_PWM_ConfigChannel(&htim3, &tim3_config, TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
}





void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

	if(htim->Instance ==  TIM2)
		{
			//Enable clock for TIM2
			*pRCC_APB1ENR_user |= (1<<0);

			//Set mode for PA0 as alternate function
			pGPIOA_user->MODER |= (0x2<<0);

			//Set medium speed for PA0 pin
			pGPIOA_user->OSPEEDER |= (1<<0);

			//Configure alternate function PA0 pin's as timer output
			pGPIOA_user->AFR[0] |= (1<<0);

		}


		else if(htim->Instance ==  TIM3)
		{

			//Enable clock for TIM3
			*pRCC_APB1ENR_user |= (1<<1);

			//Set mode for PA6 as alternate function
			pGPIOA_user->MODER |= (0x2<<12);

			//Configure alternate function PA6 pin's as timer outpu
			pGPIOA_user->AFR[0] |= (0x2<<24);

		}

}


