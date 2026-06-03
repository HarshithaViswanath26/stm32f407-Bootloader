#ifndef INC_STM32F4XX_H
#define INC_STM32F4XX_H

#include <stdint.h>

#define __vo volatile

#define STM32_IMPLEMENTED_BITS				4U

#define FLASH_BASEADDR		0x80000000U
#define SRAM1_BASEADDR		0x20000000U
#define SRAM2_BASEADDR		0x2001C000U
#define ROM_BASEADDR		0x1FFF0000U

#define SRAM_BASEADDR		SRAM1_BASEADDR

#define AHB1_BASEADDR		0x40020000U
#define APB1_BASEADDR		0x40000000U
#define APB2_BASEADDR		0x40010000U

typedef enum
{
	DISABLE = 0,
	ENABLE = 1
}FunctionalState_t;

#define SET						1
#define RESET					0

#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET

#define FLAG_SET				SET
#define FLAG_RESET				RESET
#endif /*INC_STM32F4XX_H*/
