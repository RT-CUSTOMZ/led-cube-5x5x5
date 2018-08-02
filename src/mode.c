#include "mode.h"
#include "stdio.h"

t_mode_state cube_mode = Mode_intern;

void set_mode(t_mode_state mode) {
    switch(mode) {
        case Mode_extern:
            printf("change mode to extern\n");
            break;
        case Mode_intern:
            printf("change mode to intern\n");
            break;
    }
	cube_mode = mode;
}

t_mode_state get_mode() {
	return cube_mode;
}