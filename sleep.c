#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

void do_nanos_sleep(time_t s, long ns) {
    struct timespec time_to_sleep;

    time_to_sleep.tv_sec = s;
    time_to_sleep.tv_nsec = ns;

    nanosleep(&time_to_sleep, NULL);
}

void do_micros_sleep(time_t s, long micros) {
    struct timespec time_to_sleep;

    time_to_sleep.tv_sec = s;
    time_to_sleep.tv_nsec = micros*1e3;

    nanosleep(&time_to_sleep, NULL);
}

void do_millis_sleep(time_t s, long ms) {
    struct timespec time_to_sleep;

    time_to_sleep.tv_sec = s;
    time_to_sleep.tv_nsec = ms*1e6;

    nanosleep(&time_to_sleep, NULL);
}

