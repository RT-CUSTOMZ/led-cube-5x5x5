/*
 * RGBImageMap.h
 *
 *  Created on: 09.12.2014
 *      Author: sguelland
 */

#ifndef RGBIMAGEMAP_H_
#define RGBIMAGEMAP_H_


typedef struct {
	uint16_t pixel;
	uint16_t bb_offset;
} Pixel_Mapping;

typedef struct {
	uint32_t size;
	const Pixel_Mapping *map;
	volatile uint16_t* framebuffer;
} FramebufferPixelMap;


extern const FramebufferPixelMap framebuffer_pixel_maps[];
extern const uint32_t framebuffer_pixel_maps_count;

/////////////////////intern

#include <boost/preprocessor.hpp>

#define pixel_map_bb_offset(pixel, fb_position) \
		BOOST_PP_ASSERT(BOOST_PP_LESS(fb_position,LAYERS))\
		(pixel +(24*16)*fb_position)

#define DECL_LINEAR_PIXEL_MAP(z, n, tuple) \
		{ BOOST_PP_TUPLE_ELEM(2,0,tuple) + LAYERS*LAYERS*n, pixel_map_bb_offset(BOOST_PP_TUPLE_ELEM(2,1,tuple),n)},
#define LINEAR_PIXEL_MAP(position,layer) \
		BOOST_PP_ASSERT(BOOST_PP_LESS(position,25))\
		BOOST_PP_REPEAT(LAYERS, DECL_LINEAR_PIXEL_MAP, (position, layer))


#endif /* RGBIMAGEMAP_H_ */
