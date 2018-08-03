#include "player.h"
#include "raw_sd_player.h"
#include "LED_Fading.h"

typedef struct  {
    bool (*change_effect) (uint_fast16_t number);
    void (*next_effect)   (void);
    void (*close_effect)  (void);
    bool (*render_effect) (RGBPixel* image);
} s_effect;

s_effect effects[] = {
    {
        .change_effect = NULL,
        .next_effect   = NULL,
        .close_effect  = NULL,
        .render_effect = LED_Fading_run,
    },
    {
        .change_effect = raw_sd_player_change_effect,
        .next_effect   = raw_sd_player_next_effect,
        .close_effect  = raw_sd_player_close_effect,
        .render_effect = raw_sd_player_render_effect,
    },
};

static uint32_t effect_number = 0;

bool change_effect(uint_fast16_t number) {
    if(NULL != effects[effect_number].change_effect)
        return effects[effect_number].change_effect(number);
    else
        return true;
}
void next_effect(void) {
    if(NULL != effects[effect_number].next_effect)
        effects[effect_number].next_effect();
}

void close_effect(void) {
    if(NULL != effects[effect_number].next_effect)
        effects[effect_number].close_effect();
}

bool render_effect(RGBPixel* image){
    return effects[effect_number].render_effect(image);
}

void next_module(void) {
    close_effect();
    effect_number++;
    if(effect_number >= sizeof(effects) / sizeof(effects[0])) 
        effect_number = 0;
    change_effect(0);
}