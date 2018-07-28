/*
 * cmd.h
 *
 *  Created on: 19.09.2014
 *      Author: sguelland
 */

#ifndef CMD_H_
#define CMD_H_

#include "stm32f4xx_hal.h"
#include "board.h"
#include "board_config.h"

#include "cmsis_os.h"

typedef struct {
	void (*cmd)(void* userData);
	void *userData;
} Cmd;

void CMD_Init(void);
void CMD_Input(const Cmd* cmd);

#endif /* CMD_H_ */
