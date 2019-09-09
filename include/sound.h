#ifndef __sound_h__
#define __sound_h__

#include "default_tone_raw.h"

void *sdl_handle_sound();

extern volatile bool is_sound_thread_running;
extern volatile bool is_sound_playing;

#endif
