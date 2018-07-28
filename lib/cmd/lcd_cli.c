/*
 * fd_cli.c
 *
 *  Created on: 20.09.2014
 *      Author: sguelland
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"

#include "cmsis_os.h"
#include "lcd_ks0073.h"


cparser_result_t cparser_cmd_lcd_clear(cparser_context_t *context)
{
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_lcd_clear_line_line(cparser_context_t *context,
    int32_t *line_ptr)
{
	uint_fast8_t i = 20;
	uint8_t address = *line_ptr << 5;
	LCD_SetDDRamAddress(address);
	while(i) {
		LCD_TransmitString(" ");
		i--;
	}
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_lcd_print_line_position_value(cparser_context_t *context,
    int32_t *line_ptr,
    int32_t *position_ptr,
    char **value_ptr)
{
	uint8_t address = (*line_ptr << 5) + *position_ptr;
	LCD_SetDDRamAddress(address);
	LCD_TransmitString(*value_ptr);
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_lcd_backlight_value(cparser_context_t *context,
    int32_t *value_ptr)
{
	return CPARSER_OK;
}
