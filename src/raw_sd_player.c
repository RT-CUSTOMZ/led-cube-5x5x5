#include <stdint.h>
#include "RGBImage.h"
#include "ff.h"

#include "raw_sd_player.h"

static FIL file = {0};
uint32_t effect_number = UINT32_MAX;

bool change_effect(uint_fast16_t number) {
    close_effect();

	char string_buffer[30];
	snprintf(
		string_buffer,
		sizeof(
			string_buffer
		),
		"/raw/%06d.RAW",
		number
	);

    FRESULT res = f_open (&file, string_buffer, FA_READ);
    if(FR_OK != res) {
        printf("failed to open file \n");
        return false;
    }else {
        // uint8_t image[512];
        // uint32_t read_coount;
        // res = f_lseek(&file,0);
        // res = f_read(&file, image, 512, &read_coount);
        // if(FR_OK != res) {
        //     printf("failed to open file %d\n", res);
        //     return false;
        // }
        effect_number = number;
        return true;
    }
}

void next_effect(void) {
    bool result = change_effect(effect_number +1);
    if( false == result) {
        change_effect(0);
    }
}

void close_effect(void) {
    FRESULT res;
    if(UINT32_MAX != effect_number) {
        res = f_close(&file);
        if(FR_OK != res) {
            printf("failed to close file \n");
        }
        effect_number = UINT32_MAX;
    }
}

bool render_effect(RGBPixel* image) {
    FRESULT res;
    uint_fast32_t read_coount;

    res = f_read(&file, image, 512, &read_coount);
    if(FR_OK != res) {
        printf("failed to open file \n");
        return false;
    }
    if(read_coount != 512) {
        f_lseek(&file,0);
        res = f_read(&file, image, 512, &read_coount);
    }
    return true;
}