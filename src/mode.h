#ifndef MODE_H
#define MODE_H

typedef enum e_mode_state
{
	Mode_extern,
	Mode_intern,
} t_mode_state;

void set_mode(t_mode_state mode);
t_mode_state get_mode();

#endif // MODE_H