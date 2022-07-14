/*
 * main.c
 *
 *  Created on: Jun 8, 2022
 *      Author: Jacek Twardoch
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "tim.h"
#include "motors.h"
#include "gpio.h"
#include "dma.h"
#include "usart.h"
#include "adc.h"



#define LIMIT 		2200



extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern uint32_t *pDMA2_CtrlReg_user;

volatile uint16_t buffer_1[2] = { 0, 0 };


int main(void)
{

	HAL_Init();

	SystemClock_Config();

	GPIO_Init();
	TIM2_Init();
	TIM3_Init();
	DMA_Init();
	ADC1_Init();
	USART2_UART_Init();


	char buffer[50];
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, buffer_1, 2 );
	uint8_t start =0;




	while(1)
	{
		 if((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)== 0) && ( !start))
			  {
				  start=1;
			  }

		 if(start)
		 	 {

/*---------------------------- uncomment this lines if you want check the limits values of sensors ------------------------------*/

//		 	  memset(buffer, 0, sizeof(buffer));
//			  sprintf(buffer, "ADC  RIGHT value is %d  LEFT %d\n\r", buffer_1[0], buffer_1[1]);
//			  HAL_UART_Transmit(&huart2,(uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);

/*-------------------------------------------------------------------------------------------------------------------------------*/

			  if((buffer_1[0] < LIMIT) && (buffer_1[1] < LIMIT))
				  go_straight();
			  else if((buffer_1[0] < LIMIT) && (buffer_1[1] >= LIMIT))
						 turn_left();
			  else if((buffer_1[0] >= LIMIT) && (buffer_1[1] < LIMIT))
				  turn_right();
			  else
				  stop_move();
			  }


	 HAL_Delay(20);
	}

}




void Error_Handler(void)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, GPIO_PIN_5, GPIO_PIN_SET);
}



void SystemClock_Config(void)
{

	 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);


	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 8;
	  RCC_OscInitStruct.PLL.PLLN = 76;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 2;
	  RCC_OscInitStruct.PLL.PLLR = 2;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }


//	 Initializes the CPU, AHB and APB buses clocks


	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	  {
	    Error_Handler();
	  }

}
