/*
 * LEDLava.c
 *
 *  Created on: 06.08.2014
 *      Author: sguelland
 */


#include "RGBImage.h"
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

static RGBPixel pixelBuffer[4][4][4];

static int8_t i = 3;
static int8_t x = 0;
static int8_t y = 0;
static int8_t z = 0;
static int8_t color_ = 1;
static RGBPixel color_1 = { .red = 0, .blue = 0, .green = 0, };
static RGBPixel color_2 = { .red = 0, .blue = 0, .green = 0, };

uint8_t color_lava() {

	uint8_t leave = 0;

	switch (i) {
	case 3:
		switch (color_) {
		case 1:
			color_1.red = 255;
			color_1.blue = 0;
			color_1.green = 0;
			color_ = 2;
			break;
		case 2:
			color_1.red = 255;
			color_1.blue = 0;
			color_1.green = 255;
			color_ = 3;
			break;
		case 3:
			color_1.red = 0;
			color_1.blue = 0;
			color_1.green = 255;
			color_ = 4;
			break;
		case 4:
			color_1.red = 0;
			color_1.blue = 255;
			color_1.green = 255;
			color_ = 5;
			break;
		case 5:
			color_1.red = 0;
			color_1.blue = 255;
			color_1.green = 0;
			z = 0;
			color_ = 6;
			break;
		case 6:
			color_1.red = 0;
			color_1.blue = 0;
			color_1.green = 0;
			z = 0;
			color_ = 7;
			break;

		case 7:
			leave = 1;
			color_ = 1;
			break;
		}

		i = 1;
		z = 0;
		break;
	case 1:
		if (color_1.red > 0) {
			color_1.red -= 0;
		}
		if (color_1.green > 0) {
			color_1.green -= 0;
		}
		if (color_1.blue > 0) {
			color_1.blue -= 0;
		}

		for (x = 1; x < 3; x++) {
			for (y = 1; y < 3; y++) {
				pixelBuffer[z][y][x].red = color_1.red;
				pixelBuffer[z][y][x].green = color_1.green;
				pixelBuffer[z][y][x].blue = color_1.blue;
			}
		}
		z++;
		if (z > 3) {
			i = 2;
			color_2.red = color_1.red;
			color_2.blue = color_1.blue;
			color_2.green = color_1.green;
			z = 3;
		}
		break;
	case 2:
		if (color_2.red > 0) {
			color_2.red -= 0;
		}
		if (color_2.green > 0) {
			color_2.green -= 0;
		}
		if (color_2.blue > 0) {
			color_2.blue -= 0;
		}
		y = 0;
		for (x = 0; x < 4; x++) {
			pixelBuffer[z][y][x].red = color_2.red;
			pixelBuffer[z][y][x].green = color_2.green;
			pixelBuffer[z][y][x].blue = color_2.blue;
		}
		y = 3;
		for (x = 0; x < 4; x++) {
			pixelBuffer[z][y][x].red = color_2.red;
			pixelBuffer[z][y][x].green = color_2.green;
			pixelBuffer[z][y][x].blue = color_2.blue;
		}
		x = 0;
		for (y = 1; y < 3; y++) {
			pixelBuffer[z][y][x].red = color_2.red;
			pixelBuffer[z][y][x].green = color_2.green;
			pixelBuffer[z][y][x].blue = color_2.blue;
		}
		x = 3;
		for (y = 1; y < 3; y++) {
			pixelBuffer[z][y][x].red = color_2.red;
			pixelBuffer[z][y][x].green = color_2.green;
			pixelBuffer[z][y][x].blue = color_2.blue;
		}
		z--;
		if (z < 0) {
			i = 3;
		}
		break;
	}
	return leave;
}

void LavaEffect_init(void)
{
	i = 3;
	x = 0;
	y = 0;
	z = 0;
	color_ = 1;
	memset(&color_1,0,sizeof(color_1));
	memset(&color_2,0,sizeof(color_2));
	blankPixelBuffer(pixelBuffer);
}

void LavaEffect_run(RGBPixel *pixelBuffer_unused) {
	color_lava();
	processImage((void*) &pixelBuffer);
}
