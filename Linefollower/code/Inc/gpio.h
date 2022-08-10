/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains GPIOs addresses and GPIOs registers definition
  ******************************************************************************/


#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define GPIOA_BASEADDR_user					((uint32_t*)0x40020000)
#define GPIOB_BASEADDR_user					((uint32_t*)0x40020400)
#define GPIOC_BASEADDR_user					((uint32_t*)0x40020800)
#define GPIOD_BASEADDR_user					((uint32_t*)0x40020C00)
#define GPIOE_BASEADDR_user					((uint32_t*)0x40021000)
#define GPIOF_BASEADDR_user					((uint32_t*)0x40021400)
#define GPIOG_BASEADDR_user					((uint32_t*)0x40021800)
#define GPIOH_BASEADDR_user					((uint32_t*)0x40021C00)


 typedef struct
{
	uint32_t	MODER;
	uint32_t	OTYPER;
	uint32_t	OSPEEDER;
	uint32_t	PUPDR;
	uint32_t	IDR;
	uint32_t	ODR;
	uint32_t	BSRR;
	uint32_t	LCKR;
	uint32_t	AFR[2];				// AFR[0] = AFRL ; AFR[1] = AFRH
}GPIO_Reg_user_t;

#define		GPIOA_user		((GPIO_Reg_user_t*)(GPIOA_BASEADDR_user))
#define		GPIOB_user		((GPIO_Reg_user_t*)(GPIOB_BASEADDR_user))
#define		GPIOC_user		((GPIO_Reg_user_t*)(GPIOC_BASEADDR_user))
#define		GPIOD_user		((GPIO_Reg_user_t*)(GPIOD_BASEADDR_user))
#define		GPIOE_user		((GPIO_Reg_user_t*)(GPIOE_BASEADDR_user))
#define		GPIOF_user		((GPIO_Reg_user_t*)(GPIOF_BASEADDR_user))
#define		GPIOG_user		((GPIO_Reg_user_t*)(GPIOG_BASEADDR_user))
#define		GPIOH_user		((GPIO_Reg_user_t*)(GPIOH_BASEADDR_user))

void GPIO_Init(void);

#endif	// GPIO_H_
