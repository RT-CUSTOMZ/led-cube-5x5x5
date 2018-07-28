#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"

#include "MasterControlProgram.h"

cparser_result_t cparser_cmd_effect_next(cparser_context_t *context)
{
	MCP_Command(command_next_effect);
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_effect_previous(cparser_context_t *context)
{
	MCP_Command(command_previous_effect);
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_module_next(cparser_context_t *context)
{
	MCP_Command(command_next_module);
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_module_previous(cparser_context_t *context)
{
	MCP_Command(command_previous_module);
    return CPARSER_OK;
}



/**
 * List all available commands
 */
cparser_result_t
cparser_cmd_help_filter (cparser_context_t *context, char **filter)
{
    assert(context);
    return cparser_help_cmd(context->parser, filter ? *filter : NULL);
}

cparser_result_t cparser_cmd_effect_disable(cparser_context_t *context)
{
    //cparser_cmd_show_employees(&parser->context);
	MCP_Command(command_disable_effect);
    return CPARSER_OK;
}

cparser_result_t cparser_cmd_effect_enable(cparser_context_t *context)
{
    //cparser_cmd_show_employees(&parser->context);
    return CPARSER_OK;
}


cparser_result_t cparser_cmd_motd(cparser_context_t *context)
{
	const char*message =
			"| |    ___  __| |/ ___|   _| |__   ___ \n"
			"| |   / _ \\/ _` | |  | | | | '_ \\ / _ \\\n"
			"| |__|  __/ (_| | |__| |_| | |_) |  __/\n"
			"|_____\\___|\\__,_|\\____\\__,_|_.__/ \\___|\n"
			"\n"
			"Campuswoche.de\n"
			"Version 0.1\n"
	;
	context->parser->cfg.prints(context->parser,message);
	return CPARSER_OK;
}

void RTC_CalendarShow(uint8_t* showdate);
void RTC_SetDate(uint_fast8_t year, uint_fast8_t month, uint_fast8_t day);
void RTC_SetTime(uint_fast8_t hour, uint_fast8_t minute, uint_fast8_t second);
static uint8_t aShowDate[50] = {0};

cparser_result_t cparser_cmd_rtc_date_time(cparser_context_t *context,
    char **date_ptr,
    char **time_ptr)
{
	if(date_ptr == NULL) {
		RTC_CalendarShow(aShowDate);
		context->parser->cfg.prints(context->parser,aShowDate);
		context->parser->cfg.prints(context->parser,"\n");
		return CPARSER_OK;
	}

	uint_fast8_t year;
	uint_fast8_t month;
	uint_fast8_t day;

	if( 3 != sscanf(*date_ptr,"%"PRIdLEAST8"-%"PRIdLEAST8"-%"PRIdLEAST8,
					&year, &month, &day)){
		return CPARSER_ERR_INVALID_PARAMS;
	}

	uint_fast8_t hour;
	uint_fast8_t minute;
	uint_fast8_t second;

	if( 3 != sscanf(*time_ptr,"%"PRIdLEAST8":%"PRIdLEAST8":%"PRIdLEAST8,
					&hour, &minute, &second)){
		return CPARSER_ERR_INVALID_PARAMS;
	}

	RTC_SetDate(year,month,day);
	RTC_SetTime(hour,minute,second);
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_motd2(cparser_context_t *context)
{
	return CPARSER_OK;
}


