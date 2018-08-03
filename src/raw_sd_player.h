#ifndef RAW_SD_PLAYER_H
#define RAW_SD_PLAYER_H

#include <stdint.h>
#include <stdbool.h>
#include "RGBImage.h"

bool raw_sd_player_change_effect(uint_fast16_t number);
void raw_sd_player_next_effect(void);

void raw_sd_player_close_effect(void);

bool raw_sd_player_render_effect(RGBPixel* image);

#endif //RAW_SD_PLAYER_H
