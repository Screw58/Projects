 /******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested DMA transfers.
  ******************************************************************************/

#include "dma.h"


void DMA_init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}
