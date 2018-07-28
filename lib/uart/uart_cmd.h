/*
 * uart_cmd.h
 *
 *  Created on: 01.10.2014
 *      Author: sguelland
 */

#ifndef UART_CMD_H_
#define UART_CMD_H_

#include "stdint.h"
#include "board.h"
#include "main.h"

typedef struct {
	UART_HandleTypeDef UartHandle;
	char rx_buffer[1024];
	uint32_t rx_ptr;
	void (*rxHandler)(uint8_t* data, uint32_t len, void* userPtr);
	void * rxHandlerUserPtr;
	char tx_buffer[1024];
	uint32_t tx_ptr;
	uint32_t tx_dma_ptr;
} UARTCmd;

void UARTCmd_Init(UARTCmd* cmd_data);
void UARTCmd_Tick(UARTCmd* cmd_data);
void UARTCmd_Send(uint8_t *data, uint32_t len, UARTCmd* cmd_data);

#endif /* UART_CMD_H_ */
