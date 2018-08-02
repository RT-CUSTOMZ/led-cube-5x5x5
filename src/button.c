#include "board.h"
#include "raw_sd_player.h"

#define BUTTON_SHORT_TICK_TIME	(50)
#define BUTTON_LONG_TICK_TIME	(500)

void button_init(void)
{
	KEY_BUTTON_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = KEY_BUTTON_PIN;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(KEY_BUTTON_GPIO_PORT, &GPIO_InitStruct);
}

GPIO_PinState button_state;

uint32_t last_falling_edge_tick = 0;

void button_tick(void) {
    GPIO_PinState state =  HAL_GPIO_ReadPin(KEY_BUTTON_GPIO_PORT, KEY_BUTTON_PIN);
    if(GPIO_PIN_RESET == state) {
        if(button_state == GPIO_PIN_SET) {
            // falling edge
            last_falling_edge_tick = HAL_GetTick();
        }
    }
    else {
        if(button_state == GPIO_PIN_RESET) {
            //rising edge
            if( (last_falling_edge_tick + BUTTON_LONG_TICK_TIME) <  HAL_GetTick() ) {
                //long
                next_effect();
            } else if ( (last_falling_edge_tick + BUTTON_SHORT_TICK_TIME) <  HAL_GetTick() ) {
                //short tick
                next_effect();
            }
        }
    }
    button_state = state;
}