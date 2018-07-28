/*
 * uart_cmd.c
 *
 *  Created on: 01.10.2014
 *      Author: sguelland
 */

#include "uart_cmd.h"

static HAL_StatusTypeDef HAL_UART_Receive_DMA_NO_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

void UARTCmd_Init(UARTCmd* cmd_data)
{
	cmd_data->rx_ptr = 0;

	cmd_data->UartHandle.Instance        = USARTx;
	cmd_data->UartHandle.Init.BaudRate   = 230400;//230400;115200
	cmd_data->UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	cmd_data->UartHandle.Init.StopBits   = UART_STOPBITS_1;
	cmd_data->UartHandle.Init.Parity     = UART_PARITY_NONE;
	cmd_data->UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	cmd_data->UartHandle.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&(cmd_data->UartHandle)) != HAL_OK)
	{
	   // Error_Handler();
	}
	HAL_UART_Receive_DMA_NO_IT(&(cmd_data->UartHandle), (uint8_t*)cmd_data->rx_buffer, sizeof(cmd_data->rx_buffer));
//
//	const char* message = "AT+BAUDA";
//	HAL_UART_Transmit(&(cmd_data->UartHandle), message, strlen(message), 5000);
}

void UARTCmd_Tick(UARTCmd* cmd_data)
{
//	UARTCmd_Send("tick",5,cmd_data);

	  int rx_dma_ptr = sizeof(cmd_data->rx_buffer) - __HAL_DMA_GET_COUNTER(cmd_data->UartHandle.hdmarx);
	  if(rx_dma_ptr < cmd_data->rx_ptr) {\
		  int len = sizeof(cmd_data->rx_buffer) - cmd_data->rx_ptr;
		  cmd_data->rxHandler((uint8_t*)&(cmd_data->rx_buffer[cmd_data->rx_ptr]),len,cmd_data->rxHandlerUserPtr);
		  cmd_data->rx_ptr += len;
		  if(cmd_data->rx_ptr == sizeof(cmd_data->rx_buffer))
			  cmd_data->rx_ptr = 0;
	  }
	  if(rx_dma_ptr > cmd_data->rx_ptr){
		  int len = rx_dma_ptr - cmd_data->rx_ptr;
		  cmd_data->rxHandler((uint8_t*)&(cmd_data->rx_buffer[cmd_data->rx_ptr]),len,cmd_data->rxHandlerUserPtr);
		  cmd_data->rx_ptr += len;
	  }
}

void UARTCmd_Send(uint8_t *data, uint32_t len, UARTCmd* cmd_data)
{
	HAL_UART_Transmit(&(cmd_data->UartHandle), data, len, 5000);
}


static HAL_StatusTypeDef HAL_UART_Receive_DMA_NO_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  uint32_t *tmp;
  uint32_t tmp1 = 0;

  tmp1 = huart->RxState;
  if((tmp1 == HAL_UART_STATE_READY) || (tmp1 == HAL_UART_STATE_BUSY_TX))
  {
    if((pData == NULL ) || (Size == 0))
    {
      return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(huart);

    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    /* Check if a transmit rocess is ongoing or not */
    if(huart->RxState == HAL_UART_STATE_BUSY_TX)
    {
      huart->RxState = HAL_UART_STATE_BUSY_TX_RX;
    }
    else
    {
      huart->RxState = HAL_UART_STATE_BUSY_RX;
    }

    /* Enable the DMA Stream */
    tmp = (uint32_t*)&pData;
    HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, *(uint32_t*)tmp, Size);

    /* Enable the DMA transfer for the receiver request by setting the DMAR bit
    in the UART CR3 register */
    huart->Instance->CR3 |= USART_CR3_DMAR;

    /* Process Unlocked */
    __HAL_UNLOCK(huart);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}
