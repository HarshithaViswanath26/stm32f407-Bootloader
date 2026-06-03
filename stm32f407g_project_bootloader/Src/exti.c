/*
 * exti.c
 *
 *  Created on: May 12, 2026
 *      Author: 49157
 */

#include "exti.h"
#include "stm32f4xx.h"

void Interrupt_Init(Interrupt_Handle_t* pIntHandle)
{
	// configure trigger modes
	if(pIntHandle->Exti_triggerMode == EXTI_FALLING_TRIGGER_ONLY)
	{
		EXTI->EXTI_RTSR &= ~(1 << pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum);
		EXTI->EXTI_FTSR |= (1 << pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum);
	}

	else if(pIntHandle->Exti_triggerMode == EXTI_RISING_TRIGGER_ONLY)
	{
		EXTI->EXTI_FTSR &= ~(1 << pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum);
		EXTI->EXTI_RTSR |= (1 << pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum);
	}
	else if(pIntHandle->Exti_triggerMode == EXTI_RISING_FALLING_BOTH)
	{
		EXTI->EXTI_FTSR |= (1 << pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum);
		EXTI->EXTI_RTSR |= (1 << pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum);
	}

	// configure SYSCFG_CR
	uint8_t reg = pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum/4;
	uint8_t pos = ((pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum%4) * 4);
	uint8_t val = PORT_TO_VAL(pIntHandle->pGpioHandle->GPIOx);

	SYSCFG->SYSCFG_EXTICR[reg] &= ~(0xFU << pos);
	SYSCFG->SYSCFG_EXTICR[reg] |= (val << pos);

	// configure IMR after interrupt config
	EXTI->EXTI_IMR |= (1U << pIntHandle->pGpioHandle->pinConfig.GPIO_pinNum);
}

void NVIC_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		// write 1 to enable interrupts
		if(IRQNumber < 32)
			*NVIC_ISER0 |= (1U << IRQNumber);
		else if(IRQNumber >= 32 && IRQNumber < 64)
			*NVIC_ISER1 |= (1U << IRQNumber%32);
		else if(IRQNumber >= 64 && IRQNumber < 96)
			*NVIC_ISER2 |= ( 1U << IRQNumber%32);
	}

	else if(EnorDi == DISABLE)
	{
		// write 1 to disable interrupts
		if(IRQNumber < 32)
			*NVIC_ICER0 |= (1U << IRQNumber);
		else if(IRQNumber >= 32 && IRQNumber < 64)
			*NVIC_ICER1 |= (1U << IRQNumber%32);
		else if(IRQNumber >= 64 && IRQNumber < 96)
			*NVIC_ICER2 |= ( 1U << IRQNumber%32);
	}
}

void NVIC_IRQPriorityConfig (uint8_t IRQNumber, uint8_t IRQPriority)
{
	// configure priority
	uint8_t reg = IRQNumber/4;
	uint8_t pos = (IRQNumber%4)*8;
	uint8_t bitPos = pos + (8 - STM32_IMPLEMENTED_BITS);

	*(NVIC_IPR0+reg) &= ~(0xFF << bitPos);
	*(NVIC_IPR0+reg) |= (IRQPriority << bitPos);
}

void NVIC_IRQHandling(uint8_t pinNum)
{
	// clear the pended bit after handling EXTIx
	if(EXTI->EXTI_PR & (1U << pinNum))
	{
		EXTI->EXTI_PR |= (1U << pinNum);
	}
}

uint8_t EXTI_IntStatus(uint8_t pinNum)
{
	return (EXTI->EXTI_PR & (1U << pinNum) ? 1 : 0);
}

// implement the EXTI handler wherever needed
