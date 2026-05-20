/*
 * bootloader.h
 *
 *  Created on: 19-May-2026
 *      Author: 49157
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>

#define BOOT_FLASH_ADDR				0x8000000U
#define APP_FLASH_ADDR				0x08020000U

#define SRAM_START_ADDR				0x20000000U
#define SRAM_END_ADDR				0x20020000U

#define APP_SP_ADDR					APP_FLASH_ADDR

void bootloader_JumpToApplication(void);

#endif /* BOOTLOADER_H_ */
