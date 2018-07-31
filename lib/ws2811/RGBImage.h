#ifndef RGBIMAGE_H
#define RGBIMAGE_H

#include <inttypes.h>
#include <stdbool.h>
#include "RGBFramebuffer.h"
#include "board.h"

//extern const uint16_t lookupTable[256];

typedef union _RGBPixel {
	struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
	};
	uint8_t colorByte[4];
//	uint8_t colorByte[3];
} RGBPixel;

//extern const uint8_t pixelMap[8];
extern RGBPixel rgbImage[512]; //[PIXELCOUNT];

void processImage(const RGBPixel* image);

void blankPixelBuffer(RGBPixel *pixelBuffer);

#endif //RGBIMAGE_H
