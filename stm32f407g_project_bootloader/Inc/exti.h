/*
 * exti.h
 *
 *  Created on: May 11, 2026
 *      Author: 49157
 */

#ifndef INC_EXTI_H_
#define INC_EXTI_H_

#include "stm32f4xx.h"
#include "gpio.h"

#define SYSCFG_BASEADDR						(APB2_BASEADDR + 0x3800U)
#define EXTI_BASEADDR						(APB2_BASEADDR + 0x3C00U)

// NVIC Reg Addr
#define NVIC_ISER0							((volatile uint32_t*)0xE000E100U)
#define NVIC_ISER1							((volatile uint32_t*)0xE000E104U)
#define NVIC_ISER2							((volatile uint32_t*)0xE000E108U)
#define NVIC_ISER3							((volatile uint32_t*)0xE000E10CU)

#define NVIC_ICER0							((volatile uint32_t*)0XE000E180U)
#define NVIC_ICER1							((volatile uint32_t*)0XE000E184U)
#define NVIC_ICER2							((volatile uint32_t*)0XE000E188U)
#define NVIC_ICER3							((volatile uint32_t*)0XE000E18CU)

#define NVIC_IPR0							((volatile uint32_t*)0xE000E400U)

typedef struct
{
	volatile uint32_t SYSCFG_MEMRMP;
	volatile uint32_t SYSCFG_PMC;
	volatile uint32_t SYSCFG_EXTICR[4];
	uint32_t RESERVED1[2];
	volatile uint32_t SYSCFG_CMPCR;
	//uint32_t RESERVED2[2];
	//volatile uint32_t CFGR;
}Syscfg_RegDef_t;

typedef struct
{
	volatile uint32_t EXTI_IMR;
	volatile uint32_t EXTI_EMR;
	volatile uint32_t EXTI_RTSR;
	volatile uint32_t EXTI_FTSR;
	volatile uint32_t EXTI_SWIER;
	volatile uint32_t EXTI_PR;
}Exti_RegDef_t;

#define SYSCFG					((Syscfg_RegDef_t*)SYSCFG_BASEADDR)
#define EXTI					((Exti_RegDef_t*)EXTI_BASEADDR)

typedef struct
{
	uint8_t Exti_triggerMode;
	GPIO_Handle_t* pGpioHandle;
}Interrupt_Handle_t;

// modes for RTSR & FTSR
#define EXTI_RISING_TRIGGER_ONLY				0
#define EXTI_FALLING_TRIGGER_ONLY				1
#define EXTI_RISING_FALLING_BOTH				2

// EXTI IRQ#
#define EXTI0				6
#define EXTI1				7
#define EXTI2				8
#define EXTI3				9
#define EXTI4				10

// EXTI Priority
#define EXTI0_PRIORITY		13
#define EXTI1_PRIORITY		14
#define EXTI2_PRIORITY		15
#define EXTI3_PRIORITY		16
#define EXTI4_PRIORITY		17

// values for Syscfg CR based on ports
#define PORT_TO_VAL(x)				((x==GPIOA) ? 0 :\
									(x==GPIOB) ? 1 :\
									(x==GPIOC) ? 2 :\
									(x==GPIOD) ? 3 :\
									(x==GPIOE) ? 4 :\
									(x==GPIOF) ? 5 :\
									(x==GPIOG) ? 6 :\
									(x==GPIOH) ? 7 :\
									(x==GPIOI) ? 8 :0)

void Interrupt_Init(Interrupt_Handle_t* pIntHandle);			// MCU - peripheral side
void NVIC_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);			// NVIC - processor side
void NVIC_IRQPriorityConfig (uint8_t IRQNumber, uint8_t IRQPriority);
void NVIC_IRQHandling (uint8_t PinNumber);
uint8_t EXTI_IntStatus(uint8_t pinNum);


#endif /* INC_EXTI_H_ */
