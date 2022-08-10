  /******************************************************************************
  * @file    adc.c
  * @brief   This file contains initialization function of ADC and configuration
  * 		 DMA which transmits data from sensors to buffer.
  ******************************************************************************/

#include "adc.h"



ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;


void ADC1_Init(void)
{


  ADC_ChannelConfTypeDef adc_config = {0};


  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }


  adc_config.Channel = ADC_CHANNEL_10;
  adc_config.Rank = 1;
  adc_config.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &adc_config) != HAL_OK)
  {
    Error_Handler();
  }


  adc_config.Channel = ADC_CHANNEL_11;
  adc_config.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &adc_config) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_Adc = {0};
  if(adcHandle->Instance==ADC1)
  {

    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();

//    PC0     ------> ADC1_IN10
//    PC1     ------> ADC1_IN11

    GPIO_Adc.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Adc.Mode = GPIO_MODE_ANALOG;
    GPIO_Adc.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_Adc);


    /* ADC1 DMA Init */

    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  }
}

