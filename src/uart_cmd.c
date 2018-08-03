#include "main.h"
#include "RGBFramebuffer.h"
#include <string.h>
#include <stdint.h>

#include "uart_cmd.h"
#include "mode.h"
#include "ff.h"

#include "player.h"

static uint32_t lastExternalPaketTick;
void uart_cmd_mode_watchdog(void) {
	if(Mode_extern == get_mode() && 
	   ( (lastExternalPaketTick + 10000) < HAL_GetTick() ) 
	   ){
		set_mode(Mode_intern);
	}
}

typedef enum e_uart_cmd_state
{
	CMD_START_SYNC,
	CMD_SYNC,
	CMD_START_HEADER,
	CMD_HEADER,
	CMD_START_DATA,
	CMD_DATA,
	CMD_START_CRC,
	CMD_CRC,
} t_uart_cmd_state;

static t_uart_cmd_state uart_state;
static uint16_t cmd_index;
static uint16_t cmd_crc;
static uint16_t uart_crc;

const char *const CUBE_SYNC_SEQUENCE = "CUBESYNC";

#define COMMAND_FRAME 0x0001
#define COMMAND_CMD 0x0002
typedef struct Uart_Header_s
{
	uint16_t command;
	uint16_t len;
} Uart_header_t;

#define CMD_NEXT_EFFECT 0x0001
#define CMD_PREV_EFFECT 0x0002
#define CMD_NEXT_MODULE 0x0003
#define CMD_PREV_MODULE 0x0004
#define CMD_RECORD_ON 0x0101
#define CMD_RECORD_OFF 0x0102
typedef struct Uart_Cmd_s
{
	uint16_t cmd;
} Uart_cmd_t;

static Uart_header_t uart_header;

#include "RGBImage.h"
#include "crc16ibm.h"

static bool recording = false;
static FIL saveRawImagefile;
// static uint32_t record_dir = 0;

void uart_handle_command(Uart_header_t uart_header, uint8_t *data)
{
	char string_buffer[30];
	FRESULT ret;

	switch (uart_header.command)
	{
	case COMMAND_FRAME:
		if (uart_header.len >= 375)
		{
			for (uint32_t i = 0; i < 125; i++)
			{
				rgbImage[i].red = data[i * 3 + 0];
				rgbImage[i].green = data[i * 3 + 1];
				rgbImage[i].blue = data[i * 3 + 2];
			}
			set_mode(Mode_extern);
			lastExternalPaketTick = HAL_GetTick();
			printf("Get image from WIFI\n\r");
			if(recording) {
				RGBPixel red = {
					.red   = 255,
					.green = 0,
					.blue  = 0,
				};
				unsigned int write_count;
				ret = f_write (&saveRawImagefile,(const void *) &rgbImage[0], 512, &write_count);
				if(FR_OK != ret) {
					printf("Error saving image code: %d\n\r", ret);
					rgbImage[0]  = red;
					rgbImage[4]  = red;
					rgbImage[19] = red;
					rgbImage[24] = red;
				}
				rgbImage[0] = red;
			}
			processImage((const RGBPixel*)&rgbImage);
		}
		break;
	case COMMAND_CMD:
		printf("Get cmd from WIFI\n\r");
		{
			 Uart_cmd_t* cmd_ptr = (Uart_cmd_t*) data;

			 switch(cmd_ptr->cmd)
			 {
			 case CMD_NEXT_EFFECT:
				set_mode(Mode_intern);
				next_effect();
				break;
			 case CMD_NEXT_MODULE:
				next_module();
				break;
			 case CMD_RECORD_ON: {
					recording = true;
					uint16_t record_counter = 0;
					printf("recording on\n\r");
					do{
						snprintf(string_buffer,sizeof(string_buffer), "raw/%06d.raw", record_counter);
						ret = f_open(&saveRawImagefile, string_buffer, FA_WRITE | FA_CREATE_NEW);
						record_counter++;
					} while (FR_EXIST == ret);
					if(FR_OK == ret) {
						printf("Save Image to: %s\n\r",string_buffer);
					} else {
						f_close(&saveRawImagefile);
						printf("could not open file %s for writing, code: %d\n\r", string_buffer, ret);
					}
				}
				break;
			 case CMD_RECORD_OFF:
			 {
				 recording = false;
				 printf("recording off\n\r");
				 FRESULT result = f_close(&saveRawImagefile);
				 if(FR_OK != result)
				 	printf("error f_close failed, code %d\n", result);
				 break;
			 }
			 default:
				 printf("cmd not found: %d\n\r",cmd_ptr->cmd);
				 break;
			 }
		}
		break;
	default:
		printf("Get unknown from WIFI\n\r");
		break;

	}
}
#include "RGBImage.h"

static uint8_t data_buffer[3 * 125];

void uart_cmd_input(uint8_t *data, uint32_t len, void *userPtr)
{

	for (uint32_t i = 0; i < len; i++)
	{
		uint8_t byte = data[i];
		switch (uart_state)
		{
		case CMD_START_SYNC:
			cmd_index = 0;
			uart_state = CMD_SYNC;
		case CMD_SYNC:
			if (byte == CUBE_SYNC_SEQUENCE[cmd_index])
			{
				cmd_index++;
				if (cmd_index >= 8)
					uart_state = CMD_START_HEADER;
			}
			else
			{
				cmd_index = 0;
			}
			break;
		case CMD_START_HEADER:
			cmd_index = 0;
			uart_state = CMD_HEADER;
		case CMD_HEADER:
			((uint8_t *)&uart_header)[cmd_index] = byte;
			cmd_index++;
			if (cmd_index >= 4)
			{
				if (uart_header.len > 512)
					uart_state = CMD_START_SYNC;
				else
					uart_state = CMD_START_DATA;
			}
			break;
		case CMD_START_DATA:
			cmd_index = 0;
			cmd_crc = crc16ibm_init();
			uart_state = CMD_DATA;
		case CMD_DATA:
			cmd_crc = crc16ibm_addValue(cmd_crc, byte);
			((uint8_t *)&data_buffer)[cmd_index] = byte;
			cmd_index++;
			if (cmd_index >= uart_header.len)
			{
				uart_handle_command(uart_header, (uint8_t *)&data_buffer);
				//					 debug_led_toggle();
				////					 blankPixelBuffer(rgbImage);
				//					 processImage(rgbImage);
				uart_state = CMD_START_CRC;
			}
			break;
		case CMD_START_CRC:
			cmd_index = 0;
			uart_state = CMD_CRC;
		case CMD_CRC:
			//				 crc16ibm_addValue(cmd_crc, byte);
			((uint8_t *)&uart_crc)[cmd_index] = byte;
			cmd_index++;
			if (cmd_index >= 2)
				uart_state = CMD_START_SYNC;
			break;

		default:
			uart_state = CMD_START_SYNC;
			break;
		}
	}
}
