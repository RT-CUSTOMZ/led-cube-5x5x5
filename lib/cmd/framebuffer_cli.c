#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"

#include "MasterControlProgram.h"
#include "RGBImage.h"


cparser_result_t cparser_cmd_framebuffer_blank(cparser_context_t *context)
{
	blankPixelBuffer(rgbImage);
	return CPARSER_OK;
}

static uint_fast8_t hexToNibble(char hex_value){
	if(hex_value <= '9' && hex_value >= '0')
		return hex_value - '0';
	if(hex_value <= 'F' && hex_value >= 'A' )
		return hex_value - 'A' + 10;
	if(hex_value <= 'f' && hex_value >= 'a' )
			return hex_value - 'a' + 10;
	return 0;
}

cparser_result_t cparser_cmd_framebuffer_write_address_value(cparser_context_t *context,
    uint32_t *address_ptr,
    char **value_ptr)
{
	uint_fast8_t address = *address_ptr;
	char* data = *value_ptr;
	while(true) {
		if(address > PIXELCOUNT)
			break;
		uint_fast8_t value;

		for(uint_fast8_t j = 0 ; j < 3; j++) {
			value = 0;
			if(*data == 0)
				break;
			value = hexToNibble(*data) << 4;
			data++;
			if(*data == 0)
				break;
			value |= hexToNibble(*data);
			data++;
			rgbImage[address].colorByte[j] = value;
		}

		address ++;
	}
	return CPARSER_OK;
}

#include <stdbool.h>

cparser_result_t cparser_cmd_framebuffer_refresh(cparser_context_t *context)
{
	static bool blup = false;
	if(blup == false)
	{
		MCP_Command(command_disable_effect);
		blup = true;
	}
	processImage(rgbImage);
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_framebuffer_read_address_SIZE(cparser_context_t *context,
    uint32_t *address_ptr,
    uint32_t *SIZE_ptr)
{
	return CPARSER_OK;
}
