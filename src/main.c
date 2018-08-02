#include "main.h"

#include "RGBFramebuffer.h"
#include "RGBImage.h"
#include "crc16ibm.h"
#include "uart_cmd.h"
#include "mode.h"
#include "button.h"

#include <string.h>
#include <stdint.h>

#include "raw_sd_player.h"


void SystemClock_Config(void);

void uart_cmd_input(uint8_t *data, uint32_t len, void *userPtr);
static UARTCmd uart_cmd = {
		 .rxHandler = uart_cmd_input,
		 .rxHandlerUserPtr = NULL,
};

void debug_uart_init();
void debug_uart_send(const uint8_t *data, uint32_t len);

void SD_Init(void);
void uart_cmd_mode_watchdog(void);

// static UARTCmd uart_cmd;
UART_HandleTypeDef huart;
int main(void)
{
	HAL_Init();

	/* Configure the system clock to 180 Mhz */
	SystemClock_Config();

	debug_uart_init();
	
	printf("Cube is starting...\n\r");

	HAL_UART_MspInit(&huart); //low level uart init
	UARTCmd_Init(&uart_cmd);

	initFramebuffer();
	blankPixelBuffer(rgbImage);
	rgbImage[0].red = 64;
	rgbImage[0].green = 64;
	rgbImage[0].blue = 64;
	processImage(rgbImage);

	SD_Init();
	button_init();

	// initCubeProgram();

	change_effect(0);

	/* We should never get here as control is now taken by the scheduler */
	for (;;)
	{
		uint32_t tick = HAL_GetTick();
		// HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
		UARTCmd_Tick(&uart_cmd);
		button_tick();
		uart_cmd_mode_watchdog();
		// UARTCmd_Send((uint8_t *)cmd_ping, sizeof(cmd_ping), &uart_cmd);
		// printf("ping pong\n");
		// processImage(rgbImage);
		// HAL_Delay(1);

		if( Mode_intern == get_mode() ) {
			static uint32_t nextImageTick = 50;
			if(nextImageTick <= tick) {
				nextImageTick = tick + 50;
				render_effect(rgbImage);
				processImage(rgbImage);
			}
		}
	}
}
