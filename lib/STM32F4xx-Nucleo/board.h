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

#endif /* BOARD_H_ */
