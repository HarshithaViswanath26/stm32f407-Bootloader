/*
 * usart.h
 *
 *  Created on: May 4, 2026
 *      Author: 49157
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include "stm32f4xx.h"
#include "rcc.h"
#include <stdint.h>


#define USART2_BASEADDR							(APB1_BASEADDR + 0x4400U)
#define USART3_BASEADDR							(APB1_BASEADDR + 0x4800U)
#define USART1_BASEADDR							(APB2_BASEADDR + 0x1000U)
#define USART6_BASEADDR							(APB2_BASEADDR + 0x1400U)

typedef struct
{
	volatile uint32_t USART_SR;
	volatile uint32_t USART_DR;
	volatile uint32_t USART_BRR;
	volatile uint32_t USART_CR1;
	volatile uint32_t USART_CR2;
	volatile uint32_t USART_CR3;
	volatile uint32_t USART_GTPR;
}USART_RegDef_t;

#define USART2 									((USART_RegDef_t*)USART2_BASEADDR)
#define USART3									((USART_RegDef_t*)USART3_BASEADDR)
#define USART1									((USART_RegDef_t*)USART1_BASEADDR)
#define USART6									((USART_RegDef_t*)USART6_BASEADDR)

typedef struct
{
	uint8_t USART_Mode;
	uint8_t USART_StopBits;
	uint8_t USART_WordLen;
	uint8_t USART_ParityCtrl;
	uint8_t USART_HWFlowCtrl;
	uint8_t USART_OverSampling;
	uint32_t USART_BaudRate;

}USART_Config_t;

typedef struct
{
	USART_RegDef_t* pUSARTx;
	USART_Config_t	USART_config;

}USART_Handle_t;

/*
 * @USART_Mode
 * All possible USART modes
*/
#define USART_MODE_TX					0
#define USART_MODE_RX					1
#define USART_MODE_TX_RX				2

/*
 * @USART_Stopbits
 * All possible USART stop bits as per RM
*/

#define USART_STOPBITS_1				0
#define USART_STOPBITS_0_5				1
#define USART_STOPBITS_2				2
#define USART_STOPBITS_1_5				3

/*
 * @USART_WordLen
*/

#define USART_WORDLEN_8bits				0
#define USART_WORDLEN_9bits				1

/*
 * @USART_ParityCtrl
*/
#define USART_DI_PARITY					2
#define USART_ODD_PARITY				1
#define USART_EVEN_PARITY				0

/*
 * @USART_HWFlowCtrl
*/
#define USART_HWFlowCtrl_None			0
#define USART_HWFlowCtrl_CTS			1
#define USART_HWFlowCtrl_RTS			2
#define USART_HWFlowCtrl_CTS_RTS		3

/*
 * @USART_BaudRate
*/

#define USART_BAUD_1200					1200
#define USART_BAUD_2400					2400
#define USART_BAUD_9600					9600
#define USART_BAUD_19200				19200
#define USART_BAUD_38400				38400
#define USART_BAUD_57600				57600
#define USART_BAUD_115200				115200
#define USART_BAUD_230400				230400
#define USART_BAUD_460800				460800
#define USART_BAUD_921600				921600
#define USART_BAUD_2M					2000000
#define USART_BAUD_3M					3000000

/*
 * Over-sampling scale
 */

#define USART_OVER_16					0
#define USART_OVER_8					1


/* Commonly used Flags */
#define USART_PE_FLAG					(1U << 0)
#define USART_ORE_FLAG					(1U << 3)
#define USART_IDLE_FLAG					(1U << 4)
#define USART_RXNE_FLAG					(1U << 5)
#define USART_TC_FLAG					(1U << 6)
#define USART_TXE_FLAG					(1U << 7)

/* USART2 Interrupt related */
#define USART2_IRQ						38U
#define USART2_PRIORITY					45U

/* USART APIs */

void USART_PeriClkControl(USART_RegDef_t* pUSARTx, uint8_t EnorDi);

void USART_Init(USART_Handle_t* pHandle);
void USART_DeInit(USART_RegDef_t* pUSARTx);

void USART_SetBaudRate(USART_Handle_t* pHandle);
void USART_EnableComm(USART_RegDef_t* pUSARTx);

uint8_t USART_GetFlagStatus(USART_RegDef_t* pUSARTx, uint32_t FlagName);
void USART_ClearFlagStatus(USART_RegDef_t* pUSARTx, uint32_t FlagName);

void USART_InterruptEnable(uint8_t IRQNum, uint8_t IRQPriority);

void USART_SendData(USART_Handle_t* pHandle, uint8_t* pBufferTx, uint32_t Len);
void USART_ReceiveData(USART_Handle_t* pHandle, uint8_t* pBufferRx, uint32_t Len);


#endif /* INC_USART_H_ */
