/*
 * bootloader.h
 *
 *  Created on: 19-May-2026
 *      Author: 49157
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>

// flash control reg peripheral : control
// is very much different from flash : storage
#define AHB1_BASE_ADDR				0x40020000U
#define FLASH_REG_BASE_ADDR			(0x40020000U + 0x3C00U)

// bootloader
#define BOOT_FLASH_ADDR				0x8000000U
#define APP_FLASH_ADDR				0x08020000U

// SRAM
#define SRAM_START_ADDR				0x20000000U
#define SRAM_END_ADDR				0x20020000U

#define APP_SP_ADDR					APP_FLASH_ADDR

// struct for flash inteface registers
typedef struct
{
	volatile uint32_t FLASH_ACR;
	volatile uint32_t FLASH_KEYR;
	volatile uint32_t FLASH_OPTKEYR;
	volatile uint32_t FLASH_SR;
	volatile uint32_t FLASH_CR;
	volatile uint32_t FLASH_OPTCR;

}Flash_RegDef_t;

typedef enum
{
	byte8 = 0,
	byte16 = 1,
	byte32 = 2,
	byte64 = 3
}Flash_PrgSize_t;

#define FLASH_REG					((Flash_RegDef_t*) FLASH_REG_BASE_ADDR)

// flags
#define FLASH_CR_LOCK_FLAG    		(1U << 31)
#define FLASH_BSY_FLAG				(1U << 16)

// flash sectors - 0 to 11 only erasable
#define SECTOR0						0
#define SECTOR1						1
#define SECTOR2						2
#define SECTOR3						3
#define SECTOR4						4
#define SECTOR5						5
#define SECTOR6						6
#define SECTOR7						7
#define SECTOR8						8
#define SECTOR9						9
#define SECTOR10					10
#define SECTOR11					11

// flash related APIs
uint8_t flash_Unlock(void);
uint8_t flash_Lock(void);

void flash_SectorErase(uint8_t sector);

// bootloader related APIs
void bootloader_JumpToApplication(void);

#endif /* BOOTLOADER_H_ */
