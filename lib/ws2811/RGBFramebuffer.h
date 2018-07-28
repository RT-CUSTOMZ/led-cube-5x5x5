/*
 * RGBFramebuffer.h
 *
 *  Created on: 07.05.2012
 *      Author: guelland
 */

#ifndef RGBFRAMEBUFFER_H_
#define RGBFRAMEBUFFER_H_

#include <inttypes.h>
#include <stdbool.h>
#include "board.h"

#define LAYERS 5
#define PIXELCOUNT (LAYERS*LAYERS*LAYERS)
#define PIXELSIZE 24	//in byte

extern volatile uint16_t buffer0[];
extern volatile uint16_t buffer1[];
extern volatile uint16_t buffer2[];
extern volatile uint16_t buffer3[];
extern volatile uint16_t buffer4[];
extern volatile uint16_t buffer5[];

void initFramebuffer(void);
void tickFramebuffer (void);
void blankFrameBuffer(void);

#endif /* RGBFRAMEBUFFER_H_ */
