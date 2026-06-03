/*
 * usart_test.c
 *
 *  Created on: 03-Jun-2026
 *      Author: 49157
 */


#include "usart.h"
#include "gpio.h"
#include "rcc.h"
#include "bootloader.h"


#include <stdint.h>

int main(void)
{
	USART_Handle_t usart2;
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

	while(1)
	{
		uint8_t data, sendData;
		USART_ReceiveData(&usart2, &data, sizeof(data));
		sendData = data;
		USART_SendData(&usart2, &data, sizeof(data));
	}
	return 0;
}
