#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"
#include "board.h"
#include <string.h>

cparser_result_t cparser_cmd_board_info(cparser_context_t *context)
{
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_board_serialnumber(cparser_context_t *context)
{
	char print_buffer[board_serialnumber_len + 2];
	//const unsigned int *Unique = (unsigned int *)0x1FFF7A10;

	//snprintf(print_buffer, sizeof(print_buffer),"%08X %08X %08X\n", Unique[0], Unique[1], Unique[2]) ;
	board_serialnumber(print_buffer,sizeof(print_buffer));
	int remaining_space = sizeof(print_buffer) - strlen(print_buffer) - 1;
	if(remaining_space > 0)
		strncat(print_buffer,"\n",remaining_space);
	context->parser->cfg.prints(context->parser,print_buffer);
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_board_led_led_number_cmd(cparser_context_t *context,
    char **led_number_ptr,
    char **cmd_ptr)
{
	Led_TypeDef led;

	switch(**led_number_ptr) {
	//case '3': led = LED3; break;
	//case '4': led = LED4; break;
	//case '5': led = LED5; break;
	//case '6': led = LED6; break;
	default: return CPARSER_ERR_INVALID_PARAMS;
	}

	if(strcmp(*cmd_ptr, "on") == 0)
		BSP_LED_On(led);
	else if(strcmp(*cmd_ptr, "off") == 0)
		BSP_LED_Off(led);
	else if(strcmp(*cmd_ptr, "toggle") == 0)
		BSP_LED_Toggle(led);
	else
		return CPARSER_ERR_INVALID_PARAMS;

	return CPARSER_OK;
}

cparser_result_t cparser_cmd_board_button(cparser_context_t *context)
{
	if(BSP_PB_GetState(BUTTON_KEY))
		context->parser->cfg.prints(context->parser,"on\n");
	else
		context->parser->cfg.prints(context->parser,"off\n");
	return CPARSER_OK;
}


cparser_result_t cparser_cmd_reset(cparser_context_t *context)
{
	NVIC_SystemReset();
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_bootloader(cparser_context_t *context)
{
	GOTO_BOOTLOADER();
	return CPARSER_OK;
}
