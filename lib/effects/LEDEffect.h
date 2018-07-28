/*
 * LEDEffect.h
 *
 *  Created on: 20.07.2012
 *      Author: cutter
 */

#ifndef LEDEFFECT_H_
#define LEDEFFECT_H_

#include "RGBImage.h"

void LavaEffect_init(void);
void LavaEffect_run(RGBPixel *pixelBuffer);

void layerEffect(RGBPixel *pixelBuffer);

#endif /* LEDEFFECT_H_ */
