


#include "dma.h"



void DMA_Init(void)
{
	  __HAL_RCC_DMA2_CLK_ENABLE();
}








/*
extern uint32_t *pRCC_AHB1ENR_user ;
extern volatile uint16_t buffer_1[2];

uint32_t *pDMA2_CtrlReg_user = (uint32_t*)(0x40026400+0x10);
uint32_t *pDMA2_SM0AR_user 	 = (uint32_t*)(0x40026400+0x1C);
uint32_t *pDMA2_SPAR_user 	 = (uint32_t*)(0x40026400+0x18);
uint32_t *pDMA2_S0NDTR_user  = (uint32_t*)(0x40026400+0x14);

void DMA_Init(void)
{

	//enable clock for dma2
	*pRCC_AHB1ENR_user |= (1<<22);

	//configure channel 0 of dma2
	*pDMA2_CtrlReg_user &= ~(0x7<<25);

	//psize is 16 bit
	*pDMA2_CtrlReg_user |= (0x1<<11);

	//msize is 16 bit
	*pDMA2_CtrlReg_user |= (0x1<<13);

	//transfer complete interrupt enable
	*pDMA2_CtrlReg_user |= (0x1<<4);

	//programm number of data items
	*pDMA2_S0NDTR_user |= (0x2<<0);

	//memory increment mode
	*pDMA2_CtrlReg_user |= (1<<10);

	//circular mode enabled
	*pDMA2_CtrlReg_user |= (1<<8);

	//data transfer direction
	*pDMA2_CtrlReg_user &= ~(3<<6);

	//stream 0 memory address register
	*pDMA2_SM0AR_user = buffer_1;

	//stream 0 perip address register
	*pDMA2_SPAR_user = (uint32_t) &ADC1->DR;

//--------------v hal_dma_enable v---------------//
	//dma enable
	*pDMA2_CtrlReg_user |= (1<<0);



  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}


*/
