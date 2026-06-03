#ifndef RCC_H
#define RCC_H

#include "stm32f4xx.h"
#include <stdint.h>

#define RCC_BASEADDR		(AHB1_BASEADDR + 0x3800)

// needed to enable the bit in rcc en regs
typedef enum
{
	GpioA = 0,
	GpioB = 1,
	GpioC = 2,
	GpioD = 3,
	GpioE = 4,
	GpioF = 5,
	GpioG = 6,
	GpioH = 7,
	GpioI = 8,
	CRC = 12,
	BKPSRAM = 18,
	CCMDATARAM = 20,
	DMA1 = 21,
	DMA2 = 22,
	ETHMAC = 25,
	ETHMACTX = 26,
	ETHMACRX = 27,
	ETHMACPTP = 28,
	OTGHS = 29,
	OTGHSULPI = 30
}AHB1_Peripherals_t;

typedef enum
{
	TIM2 = 0,
	TIM3 = 1,
	TIM4 = 2,
	TIM5 = 3,
	TIM6 = 4,
	TIM7 = 5,
	TIM12 = 6,
	TIM13 = 7,
	TIM14 = 8,
	WWDG = 11,
	Spi2 = 14,
	Spi3 = 15,
	Usart2 = 17,
	Usart3 = 18,
	UART4 = 19,
	UART5 = 20,
	I2C1 = 21,
	I2C2 = 22,
	I2C3 = 23,
	CAN1 = 25,
	CAN2 = 26,
	PWR = 28,
	DAC = 29
}APB1_Peripherals_t;

typedef enum
{
	TIM1 = 0,
	TIM8 = 1,
	Usart1 = 4,
	Usart6 = 5,
	ADC1 = 8,
	ADC2 = 9,
	ADC3 = 10,
	SDIO = 11,
	Spi1 = 12,
	Syscfg = 14,
	TIM9 = 16,
	TIM10 = 17,
	TIM11 = 18
}APB2_Peripherals_t;


typedef struct
{
	volatile uint32_t RCC_CR;
	volatile uint32_t RCC_PLLCFGR;
	volatile uint32_t RCC_CFGR;
	volatile uint32_t RCC_CIR;
	volatile uint32_t RCC_AHB1RSTR;
	volatile uint32_t RCC_AHB2RSTR;
	volatile uint32_t RCC_AHB3RSTR;
	uint32_t Reserved0;
	volatile uint32_t RCC_APB1RSTR;
	volatile uint32_t RCC_APB2RSTR;
	uint32_t Reserved1[2];
	volatile uint32_t RCC_AHB1ENR;
	volatile uint32_t RCC_AHB2ENR;
	volatile uint32_t RCC_AHB3ENR;
	uint32_t Reserved6;
	volatile uint32_t RCC_APB1ENR;
	volatile uint32_t RCC_APB2ENR;
	uint32_t Reserved2[2];
	volatile uint32_t RCC_AHB1LPENR;
	volatile uint32_t RCC_AHB2LPENR;
	volatile uint32_t RCC_AHB3LPENR;
	uint32_t Reserved3;
	volatile uint32_t RCC_APB1LPENR;
	volatile uint32_t RCC_APB2LPENR;
	uint32_t Reserved4[2];
	volatile uint32_t RCC_BDCR;
	volatile uint32_t RCC_CSR;
	uint32_t Reserved5[2];
	volatile uint32_t RCC_SSCGR;
	volatile uint32_t RCC_PLLI2SCFGR;
}RCC_RegDef_t;

#define RCC ((RCC_RegDef_t*) RCC_BASEADDR)

static __attribute__((unused)) uint16_t AHBPrescalar[] = {2,4,8,16,64,128,256,512};
static __attribute__((unused)) uint16_t APBxPrescalar[] = {2,4,8,16};


void RCC_AHB1_Init(AHB1_Peripherals_t peripheral, FunctionalState_t state);
void RCC_AHB1_Deinit(AHB1_Peripherals_t peripheral, FunctionalState_t state);

void RCC_APB1_Init(APB1_Peripherals_t peripheral, FunctionalState_t state);
void RCC_APB1_Deinit(APB1_Peripherals_t peripheral, FunctionalState_t state);

void RCC_APB2_Init(APB2_Peripherals_t peripheral, FunctionalState_t state);
void RCC_APB2_Deinit(APB2_Peripherals_t peripheral, FunctionalState_t state);


uint32_t RCC_GetAHBClk(void);
uint32_t RCC_GetAPB1Clk(void);
uint32_t RCC_GetAPB2Clk(void);

#endif /*RCC_H*/
