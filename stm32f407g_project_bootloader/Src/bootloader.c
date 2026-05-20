/*
 * bootloader.c
 *
 *  Created on: 19-May-2026
 *      Author: 49157
 */

#include "bootloader.h"

#include <stdint.h>
void bootloader_JumpToApplication(void)
{
	uint32_t appStartAddr = APP_FLASH_ADDR;

	uint32_t appSP = *(volatile uint32_t*)(appStartAddr);

	uint32_t appResetHandler = *(volatile uint32_t*)(appStartAddr + 4);

	if(appSP < SRAM_START_ADDR || appSP > SRAM_END_ADDR)
	{
		return;
	}

	void (*appEntry) (void) = (void (*) (void)) appResetHandler;
	// The cast (void (*)(void)) tells the compiler to treat that address as a
	// callable function pointer — without the cast, calling it would be a type error.

	__asm volatile ("MSR msp, %0" : : "r" (appSP) : );

	appEntry();

}
