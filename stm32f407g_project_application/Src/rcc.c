#include "rcc.h"
#include <stdint.h>

void RCC_AHB1_Init(AHB1_Peripherals_t peripheral, FunctionalState_t state)
{
	if(state)
		RCC->RCC_AHB1ENR |= (1U << peripheral);
}

void RCC_AHB1_Deinit(AHB1_Peripherals_t peripheral, FunctionalState_t state)
{
	if(!state)
		RCC->RCC_AHB1ENR &= ~(1U << peripheral);
}

void RCC_APB1_Init(APB1_Peripherals_t peripheral, FunctionalState_t state)
{
	if(state)
		RCC->RCC_APB1ENR |= (1U << peripheral);
}

void RCC_APB1_Deinit(APB1_Peripherals_t peripheral, FunctionalState_t state)
{
	if(!state)
		RCC->RCC_APB1ENR &= ~(1U << peripheral);
}

void RCC_APB2_Init(APB2_Peripherals_t peripheral, FunctionalState_t state)
{
	if(state)
		RCC->RCC_APB2ENR |= (1U << peripheral);
}

void RCC_APB2_Deinit(APB2_Peripherals_t peripheral, FunctionalState_t state)
{
	if(!state)
		RCC->RCC_APB2ENR &= ~(1U << peripheral);
}


/*void RCC_GetPLLVal(void)
{
	return;
}
*/
uint32_t RCC_GetAHBClk(void)
{

	uint32_t AHBClk, sysClk;
	uint8_t temp;
	uint16_t prescalar;

	// get the already set sysClk
	temp = ((RCC->RCC_CFGR >> 2) & 0x3); //fetch bits 2&3 which is sysClk
	if(temp == 0)
	{
		sysClk = 16000000;
	}
	else if(temp == 1)
	{
		sysClk = 8000000;
	}
	else if(temp == 2)
	{
		sysClk = RCC_GetPLLVal();
	}

	// get the prescalar set for sysClk wrt AHB bus: bits4,5,6,7
	temp = ((RCC->RCC_CFGR >> 4) & 0xF);
	if(temp < 8)
	{
		prescalar = 1;
	}
	else if(temp >= 8 && temp < 16)
	{
		prescalar = AHBPrescalar[temp-8];
	}

	AHBClk = sysClk/prescalar;

	return AHBClk;

}

uint32_t RCC_GetAPB1Clk(void)
{
	uint32_t AHBClk, APB1Clk;
	uint8_t temp;
	uint16_t prescalar;

	AHBClk = RCC_GetAHBClk();

	// get the APB1 prescalar set wrt AHB clk

	temp = ((RCC->RCC_CFGR >> 10) & 0x7); // fetch bits 10,11,12
	if(temp < 4)
	{
		prescalar = 1;
	}
	else if(temp >= 4 && temp < 8)
	{
		prescalar = APBxPrescalar[temp-4];
	}

	APB1Clk = AHBClk/prescalar;

    return APB1Clk;
}

uint32_t RCC_GetAPB2Clk(void)
{
	uint32_t AHBClk, APB2Clk;
	uint8_t temp;
	uint16_t prescalar;

	AHBClk = RCC_GetAHBClk();

	// get the APB1 prescalar set wrt AHB clk

	temp = ((RCC->RCC_CFGR >> 13) & 0x7); // fetch bits 13,14,15
	if(temp < 4)
	{
		prescalar = 1;
	}
	else if(temp >= 4 && temp < 8)
	{
		prescalar = APBxPrescalar[temp-4];
	}

	APB2Clk = AHBClk/prescalar;

	return APB2Clk;
}
