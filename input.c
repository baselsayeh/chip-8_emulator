#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "include/input.h"

volatile uint16_t keys_pressed = 0;
volatile bool is_input_thread_running = false; 

void *sdl_handle_keypress(void *arg) {
    SDL_Event event;
    is_input_thread_running = true;

    printf("Input thread started\n");

    while (is_input_thread_running == true) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                is_input_thread_running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_0:
                    //case SDLK_KP_0:
                        keys_pressed |= (1<<0);
                        break;

                    case SDLK_1:
                    //case SDLK_KP_1:
                        keys_pressed |= (1<<1);
                        break;

                    case SDLK_2:
                    //case SDLK_KP_2:
                        keys_pressed |= (1<<2);
                        break;

                    case SDLK_3:
                    //case SDLK_KP_3:
                        keys_pressed |= (1<<3);
                        break;

                    case SDLK_4:
                    //case SDLK_KP_4:
                        keys_pressed |= (1<<4);
                        break;

                    case SDLK_5:
                    //case SDLK_KP_5:
                        keys_pressed |= (1<<5);
                        break;

                    case SDLK_6:
                    //case SDLK_KP_6:
                        keys_pressed |= (1<<6);
                        break;

                    case SDLK_7:
                    //case SDLK_KP_7:
                        keys_pressed |= (1<<7);
                        break;

                    case SDLK_8:
                    //case SDLK_KP_8:
                        keys_pressed |= (1<<8);
                        break;

                    case SDLK_9:
                    //case SDLK_KP_9:
                        keys_pressed |= (1<<9);
                        break;

                    case SDLK_a:
                        keys_pressed |= (1<<10);
                        break;

                    case SDLK_b:
                        keys_pressed |= (1<<11);
                        break;

                    case SDLK_c:
                        keys_pressed |= (1<<12);
                        break;

                    case SDLK_d:
                        keys_pressed |= (1<<13);
                        break;

                    case SDLK_e:
                        keys_pressed |= (1<<14);
                        break;

                    case SDLK_f:
                        keys_pressed |= (1<<15);
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_0:
                        keys_pressed &= ~(1<<0);
                        break;

                    case SDLK_1:
                        keys_pressed &= ~(1<<1);
                        break;

                    case SDLK_2:
                        keys_pressed &= ~(1<<2);
                        break;

                    case SDLK_3:
                        keys_pressed &= ~(1<<3);
                        break;

                    case SDLK_4:
                        keys_pressed &= ~(1<<4);
                        break;

                    case SDLK_5:
                        keys_pressed &= ~(1<<5);
                        break;

                    case SDLK_6:
                        keys_pressed &= ~(1<<6);
                        break;

                    case SDLK_7:
                        keys_pressed &= ~(1<<7);
                        break;

                    case SDLK_8:
                        keys_pressed &= ~(1<<8);
                        break;

                    case SDLK_9:
                        keys_pressed &= ~(1<<9);
                        break;

                    case SDLK_a:
                        keys_pressed &= ~(1<<10);
                        break;

                    case SDLK_b:
                        keys_pressed &= ~(1<<11);
                        break;

                    case SDLK_c:
                        keys_pressed &= ~(1<<12);
                        break;

                    case SDLK_d:
                        keys_pressed &= ~(1<<13);
                        break;

                    case SDLK_e:
                        keys_pressed &= ~(1<<14);
                        break;

                    case SDLK_f:
                        keys_pressed &= ~(1<<15);
                        break;
                }
                break;
            break;
        }

    }

    printf("Input thread stopped\n");

}


