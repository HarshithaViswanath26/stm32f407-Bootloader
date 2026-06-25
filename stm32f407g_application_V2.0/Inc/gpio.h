#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"
#include "rcc.h"
#include <stdint.h>
#include <stdio.h>

#define GPIOA_BASEADDR		AHB1_BASEADDR
#define GPIOB_BASEADDR		(AHB1_BASEADDR + 0x0400U)
#define GPIOC_BASEADDR		(AHB1_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR		(AHB1_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR		(AHB1_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR		(AHB1_BASEADDR + 0x1400U)
#define GPIOG_BASEADDR		(AHB1_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR		(AHB1_BASEADDR + 0x1C00U)
#define GPIOI_BASEADDR		(AHB1_BASEADDR + 0x2000U)

#define GPIOPin0 	0
#define GPIOPin1 	1
#define GPIOPin2 	2
#define GPIOPin3 	3
#define GPIOPin4 	4
#define GPIOPin5 	5
#define GPIOPin6 	6
#define GPIOPin7 	7
#define GPIOPin8 	8
#define GPIOPin9 	9
#define GPIOPin10 	10
#define GPIOPin11	11
#define GPIOPin12	12
#define GPIOPin13	13
#define GPIOPin14	14
#define GPIOPin15	15


#define AF0			0
#define AF1			1
#define AF2			2
#define AF3			3
#define AF4			4
#define AF5			5
#define AF6			6
#define AF7			7
#define AF8			8
#define AF9			9
#define AF10		10
#define AF11		11
#define AF12		12
#define AF13		13
#define AF14		14
#define AF15		15

typedef struct
{
	volatile uint32_t GPIO_MODER;
	volatile uint32_t GPIO_OTYPER;
	volatile uint32_t GPIO_OSPEEDR;
	volatile uint32_t GPIO_PUPDR;
	volatile uint32_t GPIO_IDR;
	volatile uint32_t GPIO_ODR;
	volatile uint32_t GPIO_BSRR;
	volatile uint32_t GPIO_LCKR;
	volatile uint32_t GPIO_AFR[2];

}GPIO_RegDef_t;

#define GPIOA		((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB		((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC		((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD		((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE		((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF		((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG		((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH		((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI		((GPIO_RegDef_t*)GPIOI_BASEADDR)

typedef enum
{
	GPIO_input = 0,
	GPIO_output = 1,
	GPIO_alternate = 2,
	GPIO_analog = 3
}GPIO_Modes_t;

typedef enum
{
	GPIO_pushPull = 0,
	GPIO_openDrain = 1
}GPIO_Otypes_t;

typedef enum
{
	GPIO_low = 0,
	GPIO_medium = 1,
	GPIO_high = 2,
	GPIO_veryHigh = 3
}GPIO_Ospeeds_t;

typedef enum
{
	GPIO_noPupd = 0,
	GPIO_pu = 1,
	GPIO_pd = 2
}GPIO_Pupdtypes_t;


typedef struct
{
	GPIO_Modes_t GPIO_mode;
	GPIO_Otypes_t GPIO_otype;
	GPIO_Ospeeds_t GPIO_ospeed;
	GPIO_Pupdtypes_t GPIO_pupdtype;
	uint8_t GPIO_pinNum;
	uint8_t GPIO_pinAltFunMode;
}GPIO_Config_t;

typedef struct
{
	GPIO_RegDef_t* GPIOx;
	GPIO_Config_t pinConfig;

}GPIO_Handle_t;

void GPIO_Init(GPIO_Handle_t* pHandle);
//void GPIO_Clk_Enable();
void GPIO_Led_ON(GPIO_Handle_t* pHandle);
void GPIO_Led_OFF(GPIO_Handle_t* pHandle);
void GPIO_Toggle(GPIO_Handle_t* pHandle);


#endif /*GPIO_H*/
