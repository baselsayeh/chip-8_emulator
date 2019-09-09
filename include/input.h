#ifndef __input_h__
#define __input_h__

void *sdl_handle_keypress(void *arg);

extern volatile uint16_t keys_pressed;
extern volatile bool is_input_thread_running; 

#endif

