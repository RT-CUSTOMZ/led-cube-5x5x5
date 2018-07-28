/*
 * RGBImage.c
 *
 *  Created on: 04.05.2012
 *      Author: Stefan Guelland
 */

#include "RGBImage.h"
#include "RGBImageMap.h"
#include <string.h>
#include "board_config.h"

RGBPixel rgbImage[PIXELCOUNT];

//#define lookupValue(v) (lookupTable[v])

static void processPixel(RGBPixel pixel, volatile uint32_t* bb_base)
{
	uint_fast16_t bb = 0;
	for(uint_fast8_t i = 0; i<8; i++)
	{
		if( ((pixel.red >> (7-i)) &0x1 ) == 0)
			bb_base[bb] = 1;
		else
			bb_base[bb] = 0;
		bb += 16;
	}
	for(uint_fast8_t i = 0; i<8; i++)
	{
		if( ((pixel.green >> (7-i)) &0x1 ) == 0)
			bb_base[bb] = 1;
		else
			bb_base[bb] = 0;
		bb += 16;
	}
	for(uint_fast8_t i = 0; i<8; i++)
	{
		if( ((pixel.blue >> (7-i)) &0x1 ) == 0)
			bb_base[bb] = 1;
		else
			bb_base[bb] = 0;
		bb += 16;
	}
}

static const uint8_t lookup_table[] = {
		0, 0, 0, 0,
		1, 1, 1, 1,
		2, 2, 2, 2,
		3, 3, 3, 3,
		4, 4, 4, 4,
		5, 5, 5, 5,
		6, 6, 6, 6,
		7, 7, 7, 7,
		8, 8, 8, 8,
		9, 9, 9, 9,
		10, 10, 10, 10,
		11, 11, 11, 11,
		12, 12, 12, 12,
		13, 13, 13, 13,
		14, 14, 14, 14,
		16, 16, 16, 16,
		18, 18, 18, 18,
		20, 20, 20, 20,
		22, 22, 22, 22,
		25, 25, 25, 25,
		28, 28, 28, 28,
		30, 30, 30, 30,
		33, 33, 33, 33,
		36, 36, 36, 36,
		39, 39, 39, 39,
		42, 42, 42, 42,
		46, 46, 46, 46,
		49, 49, 49, 49,
		53, 53, 53, 53,
		56, 56, 56, 56,
		60, 60, 60, 60,
		64, 64, 64, 64,
		68, 68, 68, 68,
		72, 72, 72, 72,
		77, 77, 77, 77,
		81, 81, 81, 81,
		86, 86, 86, 86,
		90, 90, 90, 90,
		95, 95, 95, 95,
		100, 100, 100, 100,
		105, 105, 105, 105,
		110, 110, 110, 110,
		116, 116, 116, 116,
		121, 121, 121, 121,
		127, 127, 127, 127,
		132, 132, 132, 132,
		138, 138, 138, 138,
		144, 144, 144, 144,
		150, 150, 150, 150,
		156, 156, 156, 156,
		163, 163, 163, 163,
		169, 169, 169, 169,
		176, 176, 176, 176,
		182, 182, 182, 182,
		189, 189, 189, 189,
		196, 196, 196, 196,
		203, 203, 203, 203,
		210, 210, 210, 210,
		218, 218, 218, 218,
		225, 225, 225, 225,
		233, 233, 233, 233,
		240, 240, 240, 240,
		248, 248, 248, 248,
		255, 255, 255, 255,
};

void processImage(const RGBPixel* image) {
	// process pixel
//	GPIO_PORT_CHANNEL2->BSRRL = GPIO_PIN_CHANNEL2;

	for(uint32_t i = 0; i< framebuffer_pixel_maps_count;i++) {
		FramebufferPixelMap pixel_map = framebuffer_pixel_maps[i];
		volatile uint32_t * base = SRAM_BB_BASE + ( ((uint32_t)pixel_map.framebuffer - SRAM_BASE) * 0x20);

		for(uint_fast16_t j = 0; j < pixel_map.size ; j++){
			Pixel_Mapping pixel_mapping = pixel_map.map[j];
			RGBPixel pixel = image[pixel_mapping.pixel];
			pixel.red = lookup_table[pixel.red];
			pixel.green = lookup_table[pixel.green];
			pixel.blue = lookup_table[pixel.blue];
			processPixel(pixel,&(base[pixel_mapping.bb_offset]));
		}
	}

//	GPIO_PORT_CHANNEL2->BSRRH = GPIO_PIN_CHANNEL2;

	updateFramebuffer();
}

void blankPixelBuffer(RGBPixel *pixelBuffer)
{
	memset(pixelBuffer,0x00,sizeof(RGBPixel)*PIXELCOUNT);
}
