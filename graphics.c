#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#include "include/graphics.h"
#include "include/input.h"
#include "include/sound.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* renderer = NULL;

SDL_Rect r;
pthread_t input_thread;
pthread_t sound_thread;

void graphics_init() {
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 512, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    r.x = 0;
    r.y = 0;
    r.w = 16;
    r.h = 16;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);

    if (pthread_create(&input_thread, NULL, &sdl_handle_keypress, NULL) != 0) {
        printf("Error creating an input thread!!\n");
    }

    if (pthread_create(&input_thread, NULL, &sdl_handle_sound, NULL) != 0) {
        printf("Error creating a sound thread!!\n");
    }

}

void graphics_exit() {
    is_input_thread_running = false;
    is_sound_thread_running = false;
    do_millis_sleep(0, 500);

    pthread_cancel(input_thread);
    pthread_cancel(sound_thread);

    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void graphics_clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

//not used
/*
void graphics_draw_pixel(uint8_t x, uint8_t y, bool is_black) {
    if (is_black == true)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    r.x = (x * 16);
    r.y = (y * 16);
    r.w = 16;
    r.h = 16;

    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);
}
*/

void graphics_draw_screen(uint8_t screen[32][64]) {
    r.w = 16;
    r.h = 16;
    for (uint32_t y=0; y<32; y++) {
        for (uint32_t x=0; x<64; x++) {
            r.x = (x << 4);
            r.y = (y << 4);

            if (screen[y][x] & 0x80)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            SDL_RenderFillRect(renderer, &r);
        }
    }

    SDL_RenderPresent(renderer);
}



