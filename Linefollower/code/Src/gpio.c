/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file contains the GPIO initialization function.
  ******************************************************************************/

#include "gpio.h"


volatile GPIO_Reg_user_t  *pGPIOA_user = GPIOA_user;
volatile GPIO_Reg_user_t  *pGPIOB_user = GPIOB_user;
volatile GPIO_Reg_user_t  *pGPIOC_user = GPIOC_user;
volatile GPIO_Reg_user_t  *pGPIOD_user = GPIOD_user;
volatile GPIO_Reg_user_t  *pGPIOE_user = GPIOE_user;
volatile GPIO_Reg_user_t  *pGPIOF_user = GPIOF_user;
volatile GPIO_Reg_user_t  *pGPIOG_user = GPIOG_user;
volatile GPIO_Reg_user_t  *pGPIOH_user = GPIOH_user;

uint32_t *pRCC_AHB1ENR_user = (uint32_t*)(0x40023800+0x30);

void GPIO_Init(void)
{

	//Enable clocks for GPIOA, GPIOB, GPIOC, GPIOH
	*pRCC_AHB1ENR_user |= (0x7<<0) | (1<<7);

	// PC13(Button) pull-up enable
	pGPIOC_user->PUPDR |= (1<<26);

	//Set PB12, PB13, PB14, PB15 as output mode
	pGPIOB_user->MODER |= (1<<24) | (1<<26) | (1<<28) | (1<<30);

	//PB12, PB13, PB14, PB15 pull-down enable
	pGPIOB_user->PUPDR |= (0x2<<24) | (0x2<<26) | (0x2<<28) | (0x2<<30);

	//Configure PB12, PB13, PB14, PB15 output in low level
	pGPIOB_user->ODR &= ~(0xF<<12);


}




