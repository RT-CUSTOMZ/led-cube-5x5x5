#include "main.h"

#include "RGBFramebuffer.h"
#include "RGBImage.h"
#include "crc16ibm.h"
#include "uart_cmd.h"

#include <string.h>
#include <stdint.h>


void SystemClock_Config(void);


#define LED_PIN                                GPIO_PIN_5
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

static void LED_Init() {
  LED_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}


void uart_cmd_input(uint8_t *data, uint32_t len, void *userPtr);
static UARTCmd uart_cmd = {
		 .rxHandler = uart_cmd_input,
		 .rxHandlerUserPtr = NULL,
};

void debug_uart_init();
void debug_uart_send(const uint8_t *data, uint32_t len);

static const uint8_t cmd_ping[] = {
	'C','U','B','E','S','Y','N','C',
	0x02, 0x00, //len
	0x02, 0x00, //cmd
	0x01, 0x00,  //next effect
	'\n'
};

void SD_Init(void);

// static UARTCmd uart_cmd;
UART_HandleTypeDef huart;
int main(void)
{
	HAL_Init();
    // LED_Init();

	// debug_led_init();

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

	// initCubeProgram();

	change_effect(0);

	/* We should never get here as control is now taken by the scheduler */
	for (;;)
	{
		uint32_t tick = HAL_GetTick();
		// HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
		UARTCmd_Tick(&uart_cmd);
		// UARTCmd_Send((uint8_t *)cmd_ping, sizeof(cmd_ping), &uart_cmd);
		// printf("ping pong\n");
		// processImage(rgbImage);
		// HAL_Delay(1);

		static uint32_t nextImageTick = 50;
		if(nextImageTick <= tick) {
			nextImageTick = tick + 50;
			render_effect(rgbImage);
			processImage(rgbImage);
		}
		
	}
}
