/**
  ******************************************************************************
  * @author			: Jacek
  * @name			: Solar_tracker
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************/

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stepper_motor.h"
#include "serwo.h"



void SystemClock_Config(void);
void Recognition_func(void);

stepper_t motor1 = {0};
serwo_t serwo1={0};


uint16_t sensors_values[4];		// 0- left upper, 1- left lower, 2- right upper, 3- right lower
uint32_t left_sensors, right_sensors, upper_sensors, lower_sensors;
uint8_t  starting_flag = 1 ;
uint32_t backward_angle = 0;




int main(void)
{

  HAL_Init();


  SystemClock_Config();


  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();

  motor1.micro_steps = MICRO_STEPS_1_2 ;
  Stepper_init(&motor1, &htim3, TIM_CHANNEL_1);
  Serwo_Init(&serwo1, &htim2, TIM_CHANNEL_1);

/* set servo in starting position*/
  Serwo_set_mode(&serwo1, directly);
  Serwo_set_pos(&serwo1, 0);
  Serwo_start(&serwo1);

  HAL_ADC_Start_DMA(&hadc1, sensors_values, 4);
  Recognition_func();
  Stepper_set_mode(&motor1, continous);


  while (1)
  {
	  if( (left_sensors>right_sensors ? left_sensors-right_sensors : right_sensors-left_sensors) < DIFFERENCE)
	 	  {
	 		  Stepper_stop(&motor1);
	 	  }
	  else
	 	  {
	 		  left_sensors>right_sensors ? Stepper_set_dir(&motor1, right) : Stepper_set_dir(&motor1, left);
	 		  Stepper_start(&motor1);
	 	  }
	  if((upper_sensors>lower_sensors ? upper_sensors-lower_sensors : lower_sensors-upper_sensors) < DIFFERENCE)
	 	  {
		  	  Serwo_stop(&serwo1);
	 	  }
	  else
	 	  {
		  	  upper_sensors>lower_sensors ? Serwo_set_pos(&serwo1, SERWO_MIN_SAVE_POS) : Serwo_set_pos(&serwo1, SERWO_MAX_SAVE_POS);
		  	  Serwo_start(&serwo1);
	 	  }
	  HAL_Delay(100);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/*
 * @brief		: This function is executing only one time at the program beginning
 * @note		: This is setting a solar tracker at an angle of 45 degrees, doing one
 * 				  rotation while remembering the brightest point and at the end of this
 * 				  rotation, going back to remembered point
 */
void Recognition_func(void)
{
	 Serwo_set_mode(&serwo1, indirectly);
	 Serwo_set_pos(&serwo1, 45);
	 Serwo_set_speed(&serwo1, SERWO_SPEED_3);
	 Serwo_start(&serwo1);
	 while(serwo1.actual_position != 45);

	 Stepper_set_angle(&motor1, 360);
	 Stepper_set_dir(&motor1, left);
	 Stepper_set_mode(&motor1, angle);
	 Stepper_set_speed(&motor1, 30);
	 Stepper_start(&motor1);
	 while(starting_flag) ;
	 Stepper_set_angle(&motor1, backward_angle*(ANGLE_PER_STEP / motor1.micro_steps));
	 Stepper_set_dir(&motor1, right);
	 Stepper_start(&motor1);
	 while(motor1.angle_counter != 0) ;
}


// 0- left upper, 1- left lower, 2- right upper, 3- right lower
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	left_sensors =  sensors_values[0] + sensors_values[1];
	right_sensors = sensors_values[2] + sensors_values[3];
	upper_sensors = sensors_values[0] + sensors_values[2];
	lower_sensors = sensors_values[1] + sensors_values[3];
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim->Instance == TIM2)
	 	{
	 		Serwo_interrupt(&serwo1);
	 	}

	 if(htim->Instance == motor1.timer->Instance)
	 {
		 Stepper_interrupt(htim, &motor1);

		 /* remembering the brightest point*/
		 if(starting_flag)
		 {
			 static uint32_t point = 0xFFFFFFFF ;
			 uint32_t currently = 0;

			 currently = right_sensors + left_sensors;

			 if(currently < point)
			 {
				 point = currently;
				 backward_angle = motor1.angle_counter ;
			 }

			 if(motor1.angle_counter == 0)
				 starting_flag = 0;
		 }
	 }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }

}


