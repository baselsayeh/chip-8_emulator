#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "include/timers.h"

extern volatile uint8_t reg_delay_TIMER;
extern volatile uint8_t reg_sound_TIMER;

volatile bool is_timers_thread_running = false;
pthread_t timers_thread;

void *handle_timers(void *arg) {
    
    is_timers_thread_running = true;

    printf("Timers thread started\n");

    while (is_timers_thread_running == true) {
        do_nanos_sleep(0, 16666666);
        //printf("Slept!\n");
        if (reg_delay_TIMER > 0)
            reg_delay_TIMER--;
        if (reg_sound_TIMER > 0)
            reg_sound_TIMER--;
        else
            is_sound_playing = false;
    }

    printf("Timers thread stopped\n");
}

void init_timers() {
    //start the timers thread
    if (pthread_create(&timers_thread, NULL, &handle_timers, NULL) != 0) {
        printf("Error creating an input thread!!\n");
        exit(-1);
    }
}

void timers_exit() {
    is_timers_thread_running = false;
    do_millis_sleep(0, 250);

    pthread_cancel(timers_thread);
}
