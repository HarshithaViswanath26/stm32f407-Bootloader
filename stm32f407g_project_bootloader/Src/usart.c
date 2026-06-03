/*
 * usart.c
 *
 *  Created on: May 4, 2026
 *      Author: 49157
 */

#include "usart.h"
#include <stdint.h>


void USART_Init(USART_Handle_t* pHandle)
{
	// Configure mode
	if(pHandle->USART_config.USART_Mode == USART_MODE_TX)
	{
		pHandle->pUSARTx->USART_CR1 |= (1U << 3);
	}
	else if(pHandle->USART_config.USART_Mode == USART_MODE_RX)
	{
		pHandle->pUSARTx->USART_CR1 |= (1U << 2);
	}
	else if(pHandle->USART_config.USART_Mode == USART_MODE_TX_RX)
	{
		pHandle->pUSARTx->USART_CR1 |= (1U << 3);
		pHandle->pUSARTx->USART_CR1 |= (1U << 2);
	}

	// Configure the oversampling
	pHandle->pUSARTx->USART_CR1 |= (pHandle->USART_config.USART_OverSampling << 15);

	// Configure the stop bits
	pHandle->pUSARTx->USART_CR2 |= (pHandle->USART_config.USART_StopBits << 12);

	// Configure word len
	pHandle->pUSARTx->USART_CR1 |= (pHandle->USART_config.USART_WordLen << 12);

	// Configure parity
	// parity ctrl en & then parity select
	if(pHandle->USART_config.USART_ParityCtrl != USART_DI_PARITY)
	{
		// enable parity
		pHandle->pUSARTx->USART_CR1 |= (1U << 10);
		pHandle->pUSARTx->USART_CR1 |= (pHandle->USART_config.USART_ParityCtrl << 9);
	}

	// disable parity
	if(pHandle->USART_config.USART_ParityCtrl == USART_DI_PARITY)
	{
		pHandle->pUSARTx->USART_CR1 |= (0U << 10);
	}

	// Configure HWFlowCtrl
	if(pHandle->USART_config.USART_HWFlowCtrl == USART_HWFlowCtrl_None)
	{
		// disable both
		pHandle->pUSARTx->USART_CR3 &= ~(1U << 8); //RTS
		pHandle->pUSARTx->USART_CR3 &= ~(1U << 9); //CTS
	}
	else if(pHandle->USART_config.USART_HWFlowCtrl == USART_HWFlowCtrl_CTS)
	{
		pHandle->pUSARTx->USART_CR3 |= (1U << 9); //CTS
	}
	else if(pHandle->USART_config.USART_HWFlowCtrl == USART_HWFlowCtrl_RTS)
	{
		pHandle->pUSARTx->USART_CR3 |= (1U << 8); //RTS
	}
	else if(pHandle->USART_config.USART_HWFlowCtrl == USART_HWFlowCtrl_CTS_RTS)
	{
		// enable both
		pHandle->pUSARTx->USART_CR3 |= (1U << 8); //RTS
		pHandle->pUSARTx->USART_CR3 |= (1U << 9); //CTS
	}

	// Configure Baudrate
	USART_SetBaudRate(pHandle);

}

/*void USART_DeInit(USART_RegDef_t* pUSARTx)
{
	/*
	 * 1. Wait until TC=1
	 * 2. Disable UE
	 * 3. Reset & Release RCC peripheral clk
	 * 4. Disable the RCC peripheral clk
	 */
/*
	while(!(pUSARTx->USART_SR & (1U << 6)));
	pUSARTx->USART_CR1 &= ~(1U << 13);

	if(pUSARTx == USART1)
		USART1_REG_RESET();
	else if(pUSARTx == USART2)
		USART2_REG_RESET();
	else if(pUSARTx == USART3)
		USART3_REG_RESET();
	else if(pUSARTx == USART6)
		USART6_REG_RESET();

	USART_PeriClkControl(pUSARTx, DISABLE);


}*/

void USART_SetBaudRate(USART_Handle_t* pHandle)
{
	/*Formula for USARTDIV
	 * USARTDIV = Fclk/(8*(2-OVER8)*BR)
	 */
	uint32_t BR, Fclk;
	uint32_t usartdiv, M_part = 0, F_part = 0;

	//clear the BRR prior to setting to avoid garbage values
	pHandle->pUSARTx->USART_BRR = 0;

	BR = pHandle->USART_config.USART_BaudRate;


	if(pHandle->pUSARTx == USART1)
	{
		Fclk = RCC_GetAPB2Clk();
	}
	else if(pHandle->pUSARTx == USART6)
	{
		Fclk = RCC_GetAPB2Clk();
	}
	else if(pHandle->pUSARTx == USART2)
	{
		Fclk = RCC_GetAPB1Clk();
	}
	else if(pHandle->pUSARTx == USART3)
	{
		Fclk = RCC_GetAPB1Clk();
	}

	if(pHandle->pUSARTx->USART_CR1 & (1 << 15))
	{
		usartdiv = ((25*Fclk)/(2*BR));
	}
	else
	{
		usartdiv = ((25*Fclk)/(4*BR));
	}

	M_part = usartdiv/100;

	pHandle->pUSARTx->USART_BRR |= (M_part << 4);

	F_part = (usartdiv - (M_part*100));

	// 0x07 always results in val between 0 & 7
	if(pHandle->pUSARTx->USART_CR1 & (1 << 15))
	{
		F_part = (((F_part*8)+50)/100) & ((uint8_t)0x07);
	}
	else
	{
		F_part = (((F_part*16)+50)/100) & ((uint8_t)0x0F);
	}

	pHandle->pUSARTx->USART_BRR |= (F_part << 0);

}

uint8_t USART_GetFlagStatus(USART_RegDef_t* pUSARTx, uint32_t FlagName)
{
	return (pUSARTx->USART_SR & FlagName) ? 1 : 0;
}

void USART_ClearFlagStatus(USART_RegDef_t* pUSARTx, uint32_t FlagName)
{
	pUSARTx->USART_SR &= ~FlagName;
}

void USART_EnableComm(USART_RegDef_t* pUSARTx)
{
	//set bit13 to enable USART comm
	pUSARTx->USART_CR1 |= (1U << 13);
}

//Interrupt related APIs

/*void USART_InterruptEnable(uint8_t IRQNum, uint8_t IRQPriority)
{
	NVIC_IRQConfig(IRQNum, ENABLE);
	NVIC_IRQPriorityConfig(IRQNum,IRQPriority);
}
*/
void USART_SendData(USART_Handle_t* pHandle, uint8_t* pBufferTx, uint32_t Len)
{

	for(uint32_t i=0; i<Len; i++)
	{

		// Wait until the TXE flag is set to write the next df into DR
		while(!(pHandle->pUSARTx->USART_SR & USART_TXE_FLAG));

		if(pHandle->USART_config.USART_WordLen == USART_WORDLEN_9bits)
		{
			// extract 9 bits of data
			uint16_t* tData = (uint16_t*)pBufferTx;
			pHandle->pUSARTx->USART_DR = (*tData & (uint16_t)0x01FF);

			if(pHandle->USART_config.USART_ParityCtrl == USART_DI_PARITY)
			{
				// 9 bits df is read at once
				pBufferTx ++;
				pBufferTx++;
			}
			else
				pBufferTx++;
		}

		else if(pHandle->USART_config.USART_WordLen == USART_WORDLEN_8bits)
		{
			// extract 8 bits of data
			uint8_t* tData = (uint8_t*)pBufferTx;
			pHandle->pUSARTx->USART_DR = (*tData & (uint8_t)0xFF);

			pBufferTx++;
		}
	}

	// Wait until the data is also transmitted over the line completely
	while(!(USART_GetFlagStatus(pHandle->pUSARTx, USART_TC_FLAG)));
}

void USART_ReceiveData(USART_Handle_t* pHandle, uint8_t* pBufferRx, uint32_t Len)
{
	for(uint32_t i = 0; i < Len; i++)
	{
		// Wait till the df is completely received
		while(!(USART_GetFlagStatus(pHandle->pUSARTx, USART_RXNE_FLAG)));

		// no need to handle parity & 8/9 wordlen separately
		// parity check of bit8/7 is auto done by HW before user reads DR
		*pBufferRx = (pHandle->pUSARTx->USART_DR & 0xFF);
		pBufferRx++;

		// no need to clear RXNE flag
		// done auto by HW when read from DR
	}


}




