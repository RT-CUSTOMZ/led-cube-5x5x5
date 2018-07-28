/*
 * LEDEffect.c
 *
 *  Created on: 20.07.2012
 *      Author: cutter
 */

#include "LEDEffect.h"
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

static RGBPixel pixelBuffer[LAYERS][LAYERS][LAYERS];

void set_cube_color(uint8_t red, uint8_t green, uint8_t blue) {
	uint8_t x, y, z;
	for (x = 0; x < LAYERS; x++) {
		for (y = 0; y < LAYERS; y++) {
			for (z = 0; z < LAYERS; z++) {
				pixelBuffer[x][y][z].red = red;
				pixelBuffer[x][y][z].green = green;
				pixelBuffer[x][y][z].blue = blue;
			}
		}

	}
}

void plane_xy(RGBPixel color, uint8_t z) {
	uint8_t x, y;
	for (x = 0; x < LAYERS; x++) {
		for (y = 0; y < LAYERS; y++) {
			pixelBuffer[x][y][z].red = color.red;
			pixelBuffer[x][y][z].green = color.green;
			pixelBuffer[x][y][z].blue = color.blue;
		}

	}
}

void plane_xz(RGBPixel color, uint8_t y) {
	uint8_t x, z;
	for (x = 0; x < LAYERS; x++) {
		for (z = 0; z < LAYERS; z++) {
			pixelBuffer[x][y][z].red = color.red;
			pixelBuffer[x][y][z].green = color.green;
			pixelBuffer[x][y][z].blue = color.blue;
		}
	}
}

void plane_yz(RGBPixel color, uint8_t x) {
	uint8_t y, z;
	for (y = 0; y < LAYERS; y++) {
		for (z = 0; z < LAYERS; z++) {
			pixelBuffer[x][y][z].red = color.red;
			pixelBuffer[x][y][z].green = color.green;
			pixelBuffer[x][y][z].blue = color.blue;
		}
	}

}

uint8_t plane_move_z(RGBPixel color) {
	static uint8_t counter = 0;

	set_cube_color(0, 0, 0);

	plane_xy(color, counter);

	counter++;

	if (counter >= LAYERS) {
		counter = 0;
		return 1;
	}
	return 0;
}

uint8_t plane_move_y(RGBPixel color) {
	static uint8_t counter = 0;

	set_cube_color(0, 0, 0);

	plane_xz(color, counter);

	counter++;

	if (counter >= LAYERS) {
		counter = 0;
		return 1;
	}
	return 0;
}

uint8_t plane_move_x(RGBPixel color) {
	static uint8_t counter = 0;

	set_cube_color(0, 0, 0);

	plane_yz(color, counter);

	counter++;

	if (counter >= LAYERS) {
		counter = 0;
		return 1;
	}
	return 0;
}

uint8_t delayFunction(uint8_t (*function)(RGBPixel), RGBPixel color,
		uint32_t delay) {
	static uint32_t counter = 0;
	if (counter >= delay) {
		counter = 0;
		return (*function)(color);
	}
	counter++;
	return 0;
}


typedef struct LayerEffects {
	RGBPixel color;
	uint32_t delay;
	uint8_t (*function)(RGBPixel);
} LayerEffects;

const LayerEffects layerEffects[] = {
		{
				.delay = 20,
				.color = { .red = 0xff, .green = 0xff, .blue = 0xff, },
				.function = plane_move_x,
		},
		{
				.delay = 20,
				.color = { .red = 0xff, .green = 0xff, .blue = 0xff, },
				.function = plane_move_y,
		},
		{
				.delay = 20,
				.color = { .red = 0xff, .green = 0xff, .blue = 0xff, },
				.function = plane_move_z,
		},

		{
				.delay = 20,
				.color = { .red = 0xff, .green = 0x00, .blue = 0x00, },
				.function = plane_move_x,
		},
		{
				.delay = 20,
				.color = { .red = 0xff, .green = 0x00, .blue = 0x00, },
				.function = plane_move_y,
		},
		{
				.delay = 20,
				.color = { .red = 0xff, .green = 0x00, .blue = 0x00, },
				.function = plane_move_z,
		},

		{
				.delay = 20,
				.color = { .red = 0x00, .green = 0xff, .blue = 0x00, },
				.function = plane_move_x,
		},
		{
				.delay = 20,
				.color = { .red = 0x00, .green = 0xff, .blue = 0x00, },
				.function = plane_move_y,
		},
		{
				.delay = 20,
				.color = { .red = 0x00, .green = 0xff, .blue = 0x00, },
				.function = plane_move_z,
		},

		{
				.delay = 20,
				.color = { .red = 0x00, .green = 0x00, .blue = 0xff, },
				.function = plane_move_x,
		},
		{
				.delay = 20,
				.color = { .red = 0x00, .green = 0x00, .blue = 0xff, },
				.function = plane_move_y,
		},
		{
				.delay = 20,
				.color = { .red = 0x00, .green = 0x00, .blue = 0xff, },
				.function = plane_move_z,
		},
};

uint32_t runLayerEffect(void)
{
	static uint32_t effectCount = 0;
	LayerEffects effect;

	effect = layerEffects[effectCount];


	if(delayFunction(effect.function,effect.color,effect.delay))
		effectCount++;


	if(effectCount >= sizeof(layerEffects)/sizeof(LayerEffects))
	{
		effectCount = 0;
		return 1;
	}
	return 0;
}

void layerEffect(RGBPixel *pixelBuffer_unused) {
	//RGBPixel color = { .red = 0xff, .green = 0xff, .blue = 0xff, };
	//plane_move_y(color);
	//delayFunction(plane_move_y, color, 50);
	runLayerEffect();
	processImage((void*) &pixelBuffer);
}

