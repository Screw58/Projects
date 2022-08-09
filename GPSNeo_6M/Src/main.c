 /*
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author			: Jacek
  * @title			: GPS_Neo6
  ******************************************************************************
  */

//------------------------------ includes -------------------------------//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "NEO_6M.h"



//------------------------- global variables ---------------------------//
char circular_buffer[CIRC_BUFF_SIZE] ;
char transmit_buff[50] = {0};

static circ_buff_t buffer_one = { circular_buffer, 0, 0} ;
static NEO_state GPS = {0};

uint8_t sending_status;
volatile uint8_t lines;
char data_rx ;


//------------------------------- main ---------------------------------//
int main(void)
{

	HAL_Init();

	System_clock_config();

	GPIO_Init();
	DMA_init();
	USART2_Init();
	USART3_Init();

	uint8_t information = INCOMPLETE_INFO;
	char working_buffer[WORK_BUF_SIZE] = {0};

	HAL_UART_Receive_IT(&huart3, &data_rx, 1);

	while(1)
	{

		if(lines>0)
			  {
				  for(uint8_t i = 0 ; i < WORK_BUF_SIZE ; i++ )
				  {
				  fetch_from_buff(&buffer_one, &working_buffer[i]);
				  if(working_buffer[i] == '\n' )
					  break;
				  }
				  information = parsing_buf(working_buffer, &GPS);
				  lines--;
			  }
			  if( information == COMPLETE_INFO)
			  {

				  display_info();
				  information = INCOMPLETE_INFO;
			  }
	}
	return 0;
}

//------------------------------- end main -------------------------------//

void System_clock_config(void)
{
	RCC_OscInitTypeDef	osc_config = {0};
	RCC_ClkInitTypeDef	clk_config = {0};

	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	osc_config.OscillatorType = RCC_OSCILLATORTYPE_HSI ;
	osc_config.HSIState = RCC_HSI_ON;
	osc_config.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	osc_config.PLL.PLLSource= RCC_PLLSOURCE_HSI;
	osc_config.PLL.PLLState = RCC_PLL_ON;
	osc_config.PLL.PLLM = 16;
	osc_config.PLL.PLLN = 336 ;
	osc_config.PLL.PLLP = RCC_PLLP_DIV4 ;
	osc_config.PLL.PLLQ = 2 ;
	osc_config.PLL.PLLR = 2 ;

	if ( HAL_RCC_OscConfig(&osc_config) != HAL_OK)
	{
		Error_Handler();
	}

	clk_config.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	clk_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_config.APB1CLKDivider = RCC_HCLK_DIV2 ;
	clk_config.APB2CLKDivider = RCC_HCLK_DIV1 ;

	if ( HAL_RCC_ClockConfig(&clk_config, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}

}


void display_info(void)
{
	const char * nofix = "No fix\n\r";

	if(GPS.Quality == 0 && !sending_status)
		HAL_UART_Transmit_DMA(&huart2, nofix, strlen(nofix));

	else if(GPS.Quality == 1 && !sending_status)
	{
		sending_status = SENDING;

		memset(transmit_buff, 0, sizeof(transmit_buff));
		sprintf(transmit_buff, "\n--INFORMATION ABOUT YOUR GPS LOCATION--\r\n");
		if( HAL_UART_Transmit_DMA(&huart2, transmit_buff, strlen(transmit_buff)) != HAL_OK)
		{
			Error_Handler();
		}
	}
}


void Error_Handler(void)
{
	HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_SET);
	while(1);
}


//---------------------------- interrupt callbacks ---------------------------//

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

{
	if(huart->Instance == USART3)
	{
		if(data_rx != '\n' )
		{
			load_into_buff(&buffer_one, data_rx);
		}
		else
		{
			lines++;
			load_into_buff(&buffer_one, data_rx);
		}
		HAL_UART_Receive_IT(&huart3,&data_rx, 1);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t message_number;
	uint16_t temp=0;
	if(huart->Instance == USART2 && (sending_status))
		{

			memset(transmit_buff, 0, sizeof(transmit_buff));
			switch(message_number)
			{
				case 0 :	sprintf(transmit_buff, "UTC Time: %2d:%2d:%2d\n\r", GPS.Hour, GPS.Minute, GPS.Second );
							break;
				case 1 :	sprintf(transmit_buff, "Date: %02d-%02d-%4d\n\r", GPS.Day, GPS.Month, GPS.Year );
							break;
				case 2 :	temp = ((int)(GPS.Latitude/100))*100;
							sprintf(transmit_buff, "Latitude: %.6f %c\n\r",(int)GPS.Latitude/100 + (GPS.Latitude- temp)/60, GPS.LatitudeDirection );
							break;
				case 3 :	temp = ((int)(GPS.Longitude/100))*100;
							sprintf(transmit_buff, "Longitude: %.6f %c\n\r", (int)GPS.Longitude/100 + (GPS.Longitude - temp)/60, GPS.LongitudeDirection);
							break;
				case 4 :	sprintf(transmit_buff, "Altitude: %2.2f m above sea level\n\r", GPS.Altitude);
							break;
				case 5 :	sprintf(transmit_buff, "Speed: %.2f knots, %.5f km\\h\n\r", GPS.SpeedKnots, GPS.SpeedKilometers );
							break;
				case 6 :	sprintf(transmit_buff, "Satelites: %d\r\n", GPS.SatelitesNumber);
							break;
				case 7 :	sprintf(transmit_buff, "Dilution of precision: %.2f \n\r", GPS.Dop);
							break;
				case 8 :	sprintf(transmit_buff, "Horizontal dilution of precision:%.2f\n\r", GPS.Hdop);
							break;
				case 9 :	sprintf(transmit_buff, "Vertical dilution of precision: %.2f\n\r", GPS.Vdop );
							break;
			}
			message_number++;
			if(message_number == 10)
			{
				message_number = 0;
				sending_status = NOT_SENDING;
			}
			if( HAL_UART_Transmit_DMA(&huart2, transmit_buff, strlen(transmit_buff)) != HAL_OK)
			{
				Error_Handler();
			}
		}
}


