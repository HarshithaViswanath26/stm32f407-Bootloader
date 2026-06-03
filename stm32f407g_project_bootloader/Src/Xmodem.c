/*
 * Xmodem.c
 *
 *  Created on: 29-May-2026
 *      Author: 49157
 */

#include "Xmodem.h"
#include "usart.h"
#include "gpio.h"
#include "rcc.h"
#include "bootloader.h"


#include <stdint.h>
#include <string.h>

static USART_Handle_t usart2;

void USART_Initialize(void)
{
	// initialize GPIO pins of USART
	RCC_AHB1_Init(GpioA, ENABLE);

	GPIO_Handle_t usartTx;
	usartTx.GPIOx = GPIOA;
	usartTx.pinConfig.GPIO_pinNum = GPIOPin2;
	usartTx.pinConfig.GPIO_mode = GPIO_alternate;
	usartTx.pinConfig.GPIO_pinAltFunMode= AF7;
	usartTx.pinConfig.GPIO_otype = GPIO_pushPull;
	usartTx.pinConfig.GPIO_pupdtype = GPIO_pu;
	usartTx.pinConfig.GPIO_ospeed= GPIO_high;

	GPIO_Init(&usartTx);


	GPIO_Handle_t usartRx;
	usartRx.GPIOx = GPIOA;
	usartRx.pinConfig.GPIO_pinNum = GPIOPin3;
	usartRx.pinConfig.GPIO_mode = GPIO_alternate;
	usartRx.pinConfig.GPIO_pinAltFunMode= AF7;
	usartRx.pinConfig.GPIO_otype = GPIO_pushPull;
	usartRx.pinConfig.GPIO_pupdtype = GPIO_pu;
	usartRx.pinConfig.GPIO_ospeed= GPIO_high;

	GPIO_Init(&usartRx);

	// initialize USART
	RCC_APB1_Init(Usart2, ENABLE);

	usart2.pUSARTx = USART2;
	usart2.USART_config.USART_Mode = USART_MODE_TX_RX;
	usart2.USART_config.USART_StopBits = USART_STOPBITS_1;
	usart2.USART_config.USART_WordLen = USART_WORDLEN_8bits;
	usart2.USART_config.USART_ParityCtrl = USART_DI_PARITY;
	usart2.USART_config.USART_HWFlowCtrl = USART_HWFlowCtrl_None;
	usart2.USART_config.USART_OverSampling = USART_OVER_16;
	usart2.USART_config.USART_BaudRate = USART_BAUD_115200;


	USART_Init(&usart2);

	//extremely important!!!!!!!!!
	USART_EnableComm(USART2);


}

/*uint8_t CRC_Verify(uint8_t* data, uint8_t CRCH, uint8_t CRCL)
{

}
*/

int8_t Receive_Packet(Packet_Format_t* pHandle)
{
	/*
	 * byte by byte processing
	 * 1. Receive B1 and check for SOH/EOT
	 * 2. Receive block# as B2
	 * 3. Receive ~block# as B3
	 * 4. Verify block# ^ ~block# == 0xff
	 * 5. Receive 128B into buffer
	 * 6. Receive CRCH & CRCL
	 * 7. Verify for CRC
	 * 8. Send ACK or NACK*/

	// why double buffering not used here??


	USART_ReceiveData(&usart2, &pHandle->start, sizeof(uint8_t));

	if(pHandle->start == EOT)
	{
		return EOT_FLAG;
	}

	if(pHandle->start != SOH)
	{
		return ERROR_FLAG;
	}

	USART_ReceiveData(&usart2, &pHandle->blockNum, sizeof(uint8_t));

	USART_ReceiveData(&usart2, &pHandle->blockNumInv, sizeof(uint8_t));

	if((pHandle->blockNum ^ pHandle->blockNumInv) != 0xFF)
	{
		return ERROR_FLAG;
	}

	for(uint8_t i = 0; i < PACKET_SIZE; i++)
	{
		USART_ReceiveData(&usart2, &pHandle->data[i], sizeof(uint8_t));
	}

	USART_ReceiveData(&usart2, &pHandle->crcH, sizeof(uint8_t));
	USART_ReceiveData(&usart2, &pHandle->crcL, sizeof(uint8_t));

	uint8_t result = CRC_Verify(pHandle->data, pHandle->crcH, pHandle->crcL);

	if(!result)
	{
		return ERROR_FLAG;
	}

	return ACK_FLAG;

}

void Xmodem_Receive(void)
{
	/*
	 * 1. Initiate
	 * 2. Unlock flash
	 * 3. Erase sector 5
	 * 4. Call Receive_Packet() in a loop
	 * 5. based on return
	 * - EOT: send ACK, lock flash, break
	 * - ACK: write to flash, increement flashaddr by 128B, send ACK
	 * - Error: send NAK*/

	Packet_Format_t packet;
	uint8_t resData;
	uint8_t blockCount = 0;
	uint8_t retryCount = 0;
	uint32_t startAddr = APP_FLASH_ADDR;   //sector 5

	// initiate communication
	resData = C;
	USART_SendData(&usart2, &resData, sizeof(resData));

	flash_Unlock();
	flash_SetPrgSize();
	flash_SectorErase(SECTOR5);

	while(1)
	{
		memset(&packet, 0, sizeof(packet));
		uint8_t result;
		result = Receive_Packet(&packet);


		if(result == EOT_FLAG)
		{
			resData = ACK;
			USART_SendData(&usart2, &resData, sizeof(resData));
			flash_Lock();
			break;
		}
		else if(result == ACK_FLAG)
		{
			uint32_t alignedBuff[PACKET_SIZE/4];
			memcpy(alignedBuff, packet.data, PACKET_SIZE);
			flash_Write(startAddr, alignedBuff, PACKET_SIZE);
			startAddr += PACKET_SIZE;

			resData = ACK;
			USART_SendData(&usart2, &resData, sizeof(resData));

			blockCount++;
		}
		else if(result == ERROR_FLAG)
		{
			if(++retryCount > 10) break;

			resData = NAK;
			USART_SendData(&usart2, &resData, sizeof(resData));
		}
	}
}


