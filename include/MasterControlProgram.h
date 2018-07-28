/*
 * MasterControlProgram.h
 *
 *  Created on: 06.08.2014
 *      Author: sguelland
 */

#include "stm32f4xx_hal.h"
#include "board.h"
#include "board_config.h"

#include "cmsis_os.h"

#ifndef MASTERCONTROLPROGRAM_H_
#define MASTERCONTROLPROGRAM_H_

#define MCP_MESSAGE_NEXT_MODULE 0
#define MCP_MESSAGE_NEXT_EFFECT 1
#define MCP_MESSAGE_PREV_EFFECT 2
#define MCP_MESSAGE_DISABLE		 3

typedef struct {
		uint8_t message;
		uint8_t data0;
		uint8_t data1;
		uint8_t data2;
} MCP_MessageFormat;

extern osMessageQId  MCP_MessageQueue;

static inline void MCP_Cmd(uint_fast8_t cmd)
{
	MCP_MessageFormat message = {
			.message = cmd,
	};
	osMessagePut(MCP_MessageQueue,(int)*((int*) &message) , osWaitForever);
}

typedef enum {
	select_key,
} MCP_Key_t;

typedef enum {
	duration_short,
	duration_long,
} MCP_Duration_t;

void static inline MCP_UserInput(MCP_Key_t key, MCP_Duration_t duration)
{
	switch(key) {
	case select_key:
		if(duration == duration_short)
			MCP_Cmd(MCP_MESSAGE_NEXT_EFFECT);
		else
			MCP_Cmd(MCP_MESSAGE_NEXT_MODULE);
		break;
	default:
		break;
	}
}

typedef enum {
	command_next_effect,
	command_previous_effect,
	command_next_module,
	command_previous_module,
	command_disable_effect,
} MCP_Command_t;

void static inline MCP_Command(MCP_Command_t command)
{
	switch(command) {
		case command_next_effect:
			MCP_Cmd(MCP_MESSAGE_NEXT_EFFECT);
			break;
		case command_previous_effect:
			break;
		case command_next_module:
			MCP_Cmd(MCP_MESSAGE_NEXT_MODULE);
			break;
		case command_previous_module:
			break;
		case command_disable_effect:
			MCP_Cmd(MCP_MESSAGE_DISABLE);
			break;
	}
}

void MCP_Init(void);

#endif /* MASTERCONTROLPROGRAM_H_ */
