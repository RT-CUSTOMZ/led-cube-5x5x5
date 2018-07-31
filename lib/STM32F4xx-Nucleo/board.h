/*
 * board.h
 *
 *  Created on: 27.07.2014
 *      Author: sguelland
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "stm32f4xx_nucleo.h"
#include <inttypes.h>
#include <stdio.h>


#define board_serialnumber_len 26
static inline void board_serialnumber(char* ptr, uint32_t len){
	const uint32_t *Unique = (uint32_t *)0x1FFF7A10;
	snprintf(ptr, len,"%08"PRIX32" %08"PRIX32" %08"PRIX32"", Unique[0], Unique[1], Unique[2]) ;
}

#define BOOTLOADER_FLAG *((volatile unsigned long * const)0x2000FFF0)
#define CHECK_BOOTLOADER() \
		if(BOOTLOADER_FLAG == 0xDEADBEEF) {\
			BOOTLOADER_FLAG = 0;\
			asm (\
				"LDR        R0, =0x1FFF0000\n\t"\
				"LDR        R0,[R0, #4]\n\t"\
				"BX         R0\n\t"\
			);\
			while(1) {;}\
		}
#define GOTO_BOOTLOADER() { \
		BOOTLOADER_FLAG = 0xDEADBEEF;\
		NVIC_SystemReset();}

// Hinzugefuegt fuer sd init

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_sd.h"

#define MSD_OK         0x00
#define MSD_ERROR      0x01

#define BLOCK_SIZE                512


#define SD_CardInfo HAL_SD_CardInfoTypeDef
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef;

#define SD_DATATIMEOUT           ((uint32_t)100000000)

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)


#endif /* BOARD_H_ */


