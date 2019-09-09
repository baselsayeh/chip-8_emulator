#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
//#include <SDL2/SDL_mixer.h>

#include "include/sound.h"

volatile bool is_sound_playing = false;
volatile bool is_sound_thread_running = false; 

uint32_t audio_pos = 0;
uint32_t audio_len = 0;

void audio_callback(void *userdata, Uint8 *stream, int len) {
    if (is_sound_playing == true) {
        SDL_memcpy(stream, &default_tone_raw[audio_pos], len);

        audio_pos += len;
        if (audio_pos >= audio_len)
            audio_pos = 0;
    } else {
        SDL_memset(stream, 0, len);
        audio_pos = 0;
    }
}

void *sdl_handle_sound(void *arg) {
    SDL_AudioSpec aud_spec;

    printf("Audio thread started\n");

    audio_len = default_tone_raw_len;

    aud_spec.freq = 44100;
    aud_spec.format = AUDIO_U8;
    aud_spec.channels = 1;
    aud_spec.samples = 882;
    aud_spec.callback = audio_callback;
    aud_spec.userdata = NULL;

    if (SDL_OpenAudio(&aud_spec, NULL) != 0) {
        printf("Error opening sound!\n");
        exit(-1);
    }

    SDL_PauseAudio(0);

    is_sound_thread_running = true;

    while (is_sound_thread_running == true) {
        SDL_Delay(100); 
    }

    printf("Audio thread stopped\n");

    SDL_CloseAudio();
}

