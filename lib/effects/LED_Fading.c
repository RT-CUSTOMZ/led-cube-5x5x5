/*
 * LEDTest.c
 *
 *  Created on: 03.07.2012
 *      Author: guelland
 */

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "RGBImage.h"

#include "board.h"


static uint_fast16_t mainClockCounter = 0;
static uint16_t mixTable[LAYERS*LAYERS*LAYERS];

inline static void mainClock(void)
{
	mainClockCounter = (mainClockCounter >= 0x01ff ) ? 0x00: mainClockCounter + 0x01;
}

inline static uint8_t mainClockOffset(uint16_t offset)
{
	offset = offset & 0x01ff;
	uint16_t value = mainClockCounter + offset;

	if(value & 0x0100)
		value = ~value;
	return (uint8_t) value;
}

inline static uint8_t lowCut(uint8_t value, uint8_t cutValue)
{
	return (value > cutValue) ? value : cutValue;
}


void LED_Fading_init(void)
{
#ifdef STM32F407xx
	RNG_HandleTypeDef RngHandle;
	memset(&RngHandle,0,sizeof(RngHandle));
	RngHandle.Instance = RNG;
	RngHandle.State = HAL_RNG_STATE_RESET;
	RngHandle.Lock = HAL_UNLOCKED;

	//return;
	if(HAL_RNG_Init(&RngHandle) != HAL_OK)
	{
	/* Initialization Error */
		//Error_Handler();
	}


	uint32_t* mix_table_ptr = (uint32_t*)&mixTable;
	for(uint_fast8_t i = sizeof(mixTable) / LAYERS ; i > 0 ; i--) {
		*mix_table_ptr = HAL_RNG_GetRandomNumber(&RngHandle);
		mix_table_ptr++;
	}

	HAL_RNG_DeInit(&RngHandle);
#endif
}

bool LED_Fading_run(RGBPixel *pixelBuffer) {
	uint8_t clockValue;
	RGBPixel pixel;

	mainClock();

	for(uint_fast8_t i = 0;i < PIXELCOUNT;i++)
	{
		clockValue = mainClockOffset(mixTable[i]);
		clockValue = lowCut(clockValue, 0x0e);
		pixel.red = clockValue;
		clockValue = mainClockOffset(mixTable[i] + 170);
		clockValue = lowCut(clockValue, 0x0e);
		pixel.green = clockValue;
		clockValue = mainClockOffset(mixTable[i] + (170 *2));
		clockValue = lowCut(clockValue, 0x0e);
		pixel.blue = clockValue;

		pixelBuffer[i] = pixel;
	}
	// processImage(pixelBuffer);
	return true;
}
