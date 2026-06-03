#include "gpio.h"

void GPIO_Init(GPIO_Handle_t* pHandle)
{
    uint32_t temp = 0;

    // Configure MODE
    if(pHandle->pinConfig.GPIO_mode < GPIO_alternate)
    {
    	// only input & output mode without interrupt
		temp = pHandle->pinConfig.GPIO_mode << (2 * pHandle->pinConfig.GPIO_pinNum);
		pHandle->GPIOx->GPIO_MODER &= ~((uint32_t)0x3 << (2 * pHandle->pinConfig.GPIO_pinNum)); // clear
		pHandle->GPIOx->GPIO_MODER |= temp; // set
		temp = 0;
    }

    // Configure OTYPE
    temp =  (pHandle->pinConfig.GPIO_otype << pHandle->pinConfig.GPIO_pinNum);
    pHandle->GPIOx->GPIO_OTYPER &= ~(1 << pHandle->pinConfig.GPIO_pinNum);
    pHandle->GPIOx->GPIO_OTYPER |= temp;
    temp = 0;

    // Configure OSPEED
    temp = pHandle->pinConfig.GPIO_ospeed << (2 * pHandle->pinConfig.GPIO_pinNum);
    pHandle->GPIOx->GPIO_OSPEEDR &= ~((uint32_t)0x3 << (2 * pHandle->pinConfig.GPIO_pinNum));
    pHandle->GPIOx->GPIO_OSPEEDR |= temp;
    temp = 0;

    // configure PUPD
    temp = pHandle->pinConfig.GPIO_pupdtype << (2 * pHandle->pinConfig.GPIO_pinNum);
    pHandle->GPIOx->GPIO_PUPDR &= ~((uint32_t)0x3 << (2 * pHandle->pinConfig.GPIO_pinNum));
    pHandle->GPIOx->GPIO_PUPDR |= temp;
    temp = 0;

    // configure Alternate Functionality for a pin
    if(pHandle->pinConfig.GPIO_mode == GPIO_alternate)
    {
    	pHandle->GPIOx->GPIO_MODER |= (pHandle->pinConfig.GPIO_mode << (2*pHandle->pinConfig.GPIO_pinNum));
    	uint8_t reg = pHandle->pinConfig.GPIO_pinNum / 8;
    	uint8_t bitPos = pHandle->pinConfig.GPIO_pinNum % 8;

    	temp = (pHandle->pinConfig.GPIO_pinAltFunMode << (4*bitPos));
    	pHandle->GPIOx->GPIO_AFR[reg] |= temp;
    	temp = 0;
    }
}

void GPIO_Led_ON(GPIO_Handle_t* pHandle)
{

	pHandle->GPIOx->GPIO_ODR |= (1 << pHandle->pinConfig.GPIO_pinNum);
}

void GPIO_Led_OFF(GPIO_Handle_t* pHandle)
{
	pHandle->GPIOx->GPIO_ODR &= ~(1 << pHandle->pinConfig.GPIO_pinNum);
}

void GPIO_Toggle(GPIO_Handle_t* pHandle)
{
	pHandle->GPIOx->GPIO_ODR ^= (1 << pHandle->pinConfig.GPIO_pinNum);
}
