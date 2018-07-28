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


void LED_Fading2_init(void)
{

}

void LED_Fading2_run(RGBPixel *pixelBuffer) {

	RGBPixel pixel = {
			0xff, 0xff,0xff,0xff,
	};
	for(uint_fast8_t i = 0;i < PIXELCOUNT;i++)
	{
		pixelBuffer[i] = pixel;
	}
	processImage(pixelBuffer);
}
