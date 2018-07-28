/**
  ******************************************************************************
  * @file    usbd_cdc_if_template.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    13-June-2014
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if_template.h"
#include "cmd.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

extern USBD_HandleTypeDef USBD_Device;

#define APP_RX_DATA_SIZE  2048
#define APP_TX_DATA_SIZE  2048

uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */
uint8_t UserTxBuffer[APP_TX_DATA_SIZE];/* Received Data over UART (CDC interface) are stored in this buffer */
uint_fast16_t txSize;

/** @defgroup USBD_CDC_Private_FunctionPrototypes
  * @{
  */

static int8_t TEMPLATE_Init     (void);
static int8_t TEMPLATE_DeInit   (void);
static int8_t TEMPLATE_Control  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t TEMPLATE_Receive  (uint8_t* pbuf, uint32_t *Len);

const USBD_CDC_ItfTypeDef USBD_CDC_fops =
{
  TEMPLATE_Init,
  TEMPLATE_DeInit,
  TEMPLATE_Control,
  TEMPLATE_Receive
};

USBD_CDC_LineCodingTypeDef linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };

/* Private functions ---------------------------------------------------------*/

#include "cparser.h"
#include "cparser_tree.h"

 cparser_t parser;

/**
  * @brief  TEMPLATE_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_Init(void)
{
  /*
     Add your initialization code here
  */
    parser.cfg.root = &cparser_root;
    parser.cfg.ch_complete = '\t';
    parser.cfg.ch_erase = '\b';
    parser.cfg.ch_del = 127;
    parser.cfg.ch_help = '?';
    parser.cfg.flags = 0;//CPARSER_FLAGS_DEBUG : 0);
    strcpy(parser.cfg.prompt, "LedCube>> ");
    cparser_io_config(&parser);
    cparser_init(&parser.cfg, &parser);
    //cparser_fsm_reset(&parser);
    parser.cfg.io_init(&parser);
    //cparser_print_prompt(&parser);
    parser.done = 0;
	  /*##-5- Set Application Buffers ############################################*/
	  USBD_CDC_SetTxBuffer(&USBD_Device, UserTxBuffer, 0);
	  USBD_CDC_SetRxBuffer(&USBD_Device, UserRxBuffer);
	  txSize = 0;
  return (0);
}

/**
  * @brief  TEMPLATE_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_DeInit(void)
{
  /*
     Add your deinitialization code here
  */
  return (0);
}


/**
  * @brief  TEMPLATE_Control
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_Control  (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{ 
  switch (cmd)
  {
  case CDC_SEND_ENCAPSULATED_COMMAND:
    /* Add your code here */
    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:
    /* Add your code here */
    break;

  case CDC_SET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_GET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_CLEAR_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_SET_LINE_CODING:
    linecoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
                            (pbuf[2] << 16) | (pbuf[3] << 24));
    linecoding.format     = pbuf[4];
    linecoding.paritytype = pbuf[5];
    linecoding.datatype   = pbuf[6];
    
    /* Add your code here */
    break;

  case CDC_GET_LINE_CODING:
    pbuf[0] = (uint8_t)(linecoding.bitrate);
    pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
    pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
    pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
    pbuf[4] = linecoding.format;
    pbuf[5] = linecoding.paritytype;
    pbuf[6] = linecoding.datatype;

    /* Add your code here */
    break;

  case CDC_SET_CONTROL_LINE_STATE:
    /* Add your code here */
    break;

  case CDC_SEND_BREAK:
     /* Add your code here */
    break;    
    
  default:
    break;
  }

  return (0);
}

void cdc_print(char* ptr, uint32_t size)
{
	for(; size;size--) {
		UserTxBuffer[txSize] = *ptr;
		if(*ptr == '\n') {
			txSize++;
			UserTxBuffer[txSize] = '\r';
		}
		ptr++;
		txSize++;
	}
}


typedef struct ReceiveData {
	uint8_t* buffer;
	uint32_t length;
} ReceiveData;
static ReceiveData receiveData;
static void receive(void *userData) {
	uint32_t len = ((ReceiveData*)userData)->length;
	uint8_t* buffer = ((ReceiveData*)userData)->buffer;

	for(;len;len--)
	{
		if(*buffer == 0x0D)
			*buffer = '\n';
		cparser_input(&parser, *buffer, CPARSER_CHAR_REGULAR);
		buffer++;
	}
	USBD_CDC_ReceivePacket(&USBD_Device);
	if(txSize) {
		taskENTER_CRITICAL();
	    USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t*)UserTxBuffer, txSize);
	    USBD_CDC_TransmitPacket(&USBD_Device);
	    taskEXIT_CRITICAL();
	    txSize = 0;
	}
}
static const Cmd receive_cmd = {
	.cmd = receive,
	.userData = &receiveData,
};

/**
  * @brief  TEMPLATE_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_Receive (uint8_t* Buf, uint32_t *Len)
{
	receiveData.buffer = Buf;
	receiveData.length = *Len;
	CMD_Input(&receive_cmd);
//	uint32_t len = *Len;
//	//cparser_result_t cparser_input(cparser_t *parser, char ch, cparser_char_t ch_type);
//	for(;len;len--)
//	{
//		if(*Buf == 0x0D)
//			*Buf = '\n';
//		cparser_input(&parser, *Buf, CPARSER_CHAR_REGULAR);
//		Buf++;
//	}
//	USBD_CDC_ReceivePacket(&USBD_Device);
//	if(txSize) {
//	    USBD_CDC_SetTxBuffer(&USBD_Device, (uint8_t*)UserTxBuffer, txSize);
//	    USBD_CDC_TransmitPacket(&USBD_Device);
//	    txSize = 0;
//	}
	return (USBD_OK);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

