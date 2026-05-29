/*
 * bootloader.c
 *
 *  Created on: 19-May-2026
 *      Author: 49157
 */

#include "bootloader.h"

#include <stdint.h>

uint8_t flash_Unlock(void)
{
	/*
	 * 1. Unlock FLASH_KEYR: proper sequence
	 * 2. Check for lock bit in CR to be 0
	 * lock bit is auto cleared by HW after unlock */
	while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);
	FLASH_REG->FLASH_KEYR =  0x45670123;
	FLASH_REG->FLASH_KEYR =   0xCDEF89AB;
	// above is a whole 32bit sequence, no need to shift by 16 for second

	uint32_t timeOut = 100000U;
	while((FLASH_REG->FLASH_CR & (FLASH_CR_LOCK_FLAG)) && timeOut--);

	return (!(FLASH_REG->FLASH_CR & (FLASH_CR_LOCK_FLAG))); // no need to cache here
}

uint8_t flash_Lock(void)
{
	/* 1. Continuously poll the BSY bit for 0
	 * 2. Write lock bit = 1
	 * */

	while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);

	FLASH_REG->FLASH_CR |= (1U << 31);

	return 1;
}

void flash_SetPrgSize(void)
{
	while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);

	FLASH_REG->FLASH_CR &= ~(0x3 << 8);
	FLASH_REG->FLASH_CR |= (byte32 << 8);
	// for 3.3Vdd 8, 16 & 32 all possible
	// select highest to cut down on CPU stall time

	//while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG); // stall CPU to complete op
	// need not stall here: no op only bit setting

}

void flash_SectorErase(uint8_t sector)
{
	// !! Never erase sector 0 since thats where bootloader is
	// so the bootloader code gets broken, and halts mid way
	/* 1. Poll BSY, wait until all ops over
	 * 2. Set SER and select the sector to be erased
	 * -- always set the PSIZE for any op in Flash - both write/erase
	 * 3. Set STRT bit
	 * 4. Poll until BSY is cleared */
	while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);
	// wait for any ongoing ops to finish

	FLASH_REG->FLASH_CR |= (1U << 1);

	FLASH_REG->FLASH_CR &= ~(0xF << 3);
	FLASH_REG->FLASH_CR |= (sector << 3);

	FLASH_REG->FLASH_CR |= (1U << 16);


	while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG); // stall CPU to complete erase
	FLASH_REG->FLASH_CR &= ~(1U << 1);  // clear SER
}

void flash_Write(uint32_t addr, uint32_t* data, uint32_t len)
{
	while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);
	//set PG
	FLASH_REG->FLASH_CR |= (1U << 0);

	//uint32_t data = 0x12345678;
	for(uint32_t i = 0; i < len/4; i++)
	{
		*(volatile uint32_t*)(addr + (i*4)) = data[i];
		while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);
	}

	FLASH_REG->FLASH_CR &= ~(1U << 0);

}

void flash_Read(uint32_t addr, uint32_t* buffer, uint32_t len)
{
	while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);

	for(uint32_t i = 0; i < len/4; i++)
	{
		buffer[i] = *(volatile uint32_t*)(addr + (i*4));
		while(FLASH_REG->FLASH_SR & FLASH_BSY_FLAG);
	}
}


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




