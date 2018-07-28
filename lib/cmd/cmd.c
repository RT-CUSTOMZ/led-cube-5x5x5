/*
 * cmd.c
 *
 *  Created on: 19.09.2014
 *      Author: sguelland
 */

#include "cmd.h"


osMessageQId  CMD_InputQueue;
static osThreadId CMD_ThreadHandler;
static void CMD_Thread(void const *argument);


static osMessageQDef(CMD_input,5,Cmd*);
static osThreadDef(CMD, CMD_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE + 512);


void CMD_Init(void)
{
	CMD_InputQueue = osMessageCreate(osMessageQ(CMD_input), NULL);
//	CMD_ThreadHandler = osThreadCreate (osThread(CMD), NULL);
}

void CMD_Thread(void const *argument)
{
	for(;;) {
		osEvent inputEvent;
		inputEvent = osMessageGet(CMD_InputQueue, osWaitForever);
		Cmd* cmd = inputEvent.value.p;
		cmd->cmd(cmd->userData);
	}
}

void CMD_Input(const Cmd* cmd)
{
	osMessagePut(CMD_InputQueue,(int)cmd , 0);
}
