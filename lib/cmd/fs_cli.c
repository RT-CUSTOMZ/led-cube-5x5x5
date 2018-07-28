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
#include "ff.h"

cparser_result_t cparser_cmd_ls_path(cparser_context_t *context,
    char **path_ptr)
{
	FRESULT ret;
	FILINFO fno;
	DIR dir;
	char buffer[50];
	char* directory;

	if(path_ptr == NULL)
		directory = "/";
	else
		directory = *path_ptr;

	ret = f_opendir(&dir, directory);
	if (ret)
	{
		snprintf(buffer,sizeof(buffer),"Open %s directory error\n\r", directory);
		context->parser->cfg.prints(context->parser,buffer);
		return CPARSER_ERR_NOT_EXIST;
	}
	while(1){
		//taskENTER_CRITICAL();
		ret = f_readdir(&dir, &fno);		// Read a directory item
		//taskEXIT_CRITICAL();
		if (ret || !fno.fname[0])
			break;	// Error or end of dir
		snprintf(buffer,sizeof(buffer),"%s \n", fno.fname);
		context->parser->cfg.prints(context->parser,buffer);
	}
	//taskENTER_CRITICAL();
	f_closedir(&dir);
	//taskEXIT_CRITICAL();
	return CPARSER_OK;
}

cparser_result_t cparser_cmd_cd_path(cparser_context_t *context,
    char **path_ptr)
{

	return CPARSER_OK;
}

cparser_result_t cparser_cmd_cat_path(cparser_context_t *context,
    char **path_ptr)
{

	return CPARSER_OK;
}

