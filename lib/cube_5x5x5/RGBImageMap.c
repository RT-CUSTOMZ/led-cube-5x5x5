/*
 * RGBImageMap.c
 *
 *  Created on: 09.12.2014
 *      Author: sguelland
 */

#include "RGBImage.h"
#include "RGBImageMap.h"
#include <string.h>
#include "board_config.h"



static const Pixel_Mapping framebuffer0_pixel_map[] = {
		LINEAR_PIXEL_MAP( 0,  0) // 1.1
		LINEAR_PIXEL_MAP( 16,  1) // 4.2
		LINEAR_PIXEL_MAP( 21,  4) // 5.2
		LINEAR_PIXEL_MAP( 3,  5) // 1.4
		LINEAR_PIXEL_MAP( 18,  6) // 4.4
		LINEAR_PIXEL_MAP( 6,  7) // 2.2
		LINEAR_PIXEL_MAP( 12,  8) // 3.3
		LINEAR_PIXEL_MAP( 13,  9) // 1.4
		LINEAR_PIXEL_MAP(23, 10) // 4.5 ////
		LINEAR_PIXEL_MAP( 10, 12) // 3.2
		LINEAR_PIXEL_MAP(20, 13) // 5.2
		LINEAR_PIXEL_MAP(24, 14) // 5.3
		LINEAR_PIXEL_MAP(22, 15) // 5.5
};

static const Pixel_Mapping framebuffer1_pixel_map[] = {
		LINEAR_PIXEL_MAP(9,  0) // 2.5
		LINEAR_PIXEL_MAP(8,  1) // 2.4
		LINEAR_PIXEL_MAP(7,  4) // 2.3
		LINEAR_PIXEL_MAP(11,  6) //3.4
		LINEAR_PIXEL_MAP(17,  7) // 4.3
		LINEAR_PIXEL_MAP(15,  8) // 4.3
		LINEAR_PIXEL_MAP(5,  15) // 2.1
};

static const Pixel_Mapping framebuffer2_pixel_map[] = {
		LINEAR_PIXEL_MAP(4,  0) //1.5
		LINEAR_PIXEL_MAP(1,  1) //1.2
		LINEAR_PIXEL_MAP(2,  4) // 1.3
		LINEAR_PIXEL_MAP(14,  5) //3.5
		LINEAR_PIXEL_MAP(19,  7) //4.5
};


const FramebufferPixelMap framebuffer_pixel_maps[] = {
		{
			.size = sizeof(framebuffer0_pixel_map) / sizeof(framebuffer0_pixel_map[1]),
			.map = framebuffer0_pixel_map,
			.framebuffer = buffer0,
		},
		{
			.size = sizeof(framebuffer1_pixel_map) / sizeof(framebuffer1_pixel_map[1]),
			.map = framebuffer1_pixel_map,
			.framebuffer = buffer1,
		},
		{
			.size = sizeof(framebuffer2_pixel_map) / sizeof(framebuffer2_pixel_map[1]),
			.map = framebuffer2_pixel_map,
			.framebuffer = buffer2,
		},
};

const uint32_t framebuffer_pixel_maps_count = sizeof(framebuffer_pixel_maps) / sizeof(framebuffer_pixel_maps[0]);
