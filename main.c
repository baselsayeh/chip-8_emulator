#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "include/graphics.h"
#include "include/sleep.h"
#include "include/timers.h"

#include "include/default_sprite_data.h"

//
#include "mem.c"
#include "cpu.c"
#include "graphics.c"
#include "input.c"
#include "sleep.c"
#include "sound.c"
#include "timers.c"

int main(int argc, char *argv[]) {
    FILE *fp;
    long int f_size;
    char buf;

    //
    printf("Chip-8 interpreter v1.0\n");
    printf("Github: Baselsayeh\n");
    do_millis_sleep(0, 500);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    //open the file
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file!\n");
        return 1;
    }

    //get file size
    fseek(fp, 0L, SEEK_END);
    f_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    /*if (f_size & 0x01) {
        printf("Warning: ignoring last byte\n");
        f_size &= (~0x1);
    }*/

    //init things
    init_cpu();
    init_mem();
    graphics_init();
    init_timers();

    //load a program here
    printf("Loading program...\n");
    for (uint32_t i=0; i<f_size; i++) {
        if (fread(&buf, 1, 1, fp) != 1) {
            fprintf(stderr, "Error reading file\n");
            fclose(fp);
            goto exit;
        }
        mem_store(0x0200 + i, buf, 1);
    }
    fclose(fp);
    printf("Finished loading program\n");

    //store number sprite data
    printf("Initializing sprite data...\n");
    for (long int i=0; i<sizeof(d_sprite); i++) {
        mem_store(0x0000 + i, d_sprite[i], 1);
    }
    printf("Initialized sprite data\n");

    //start the cpu main loop
    printf("Executing program...\n\n\n");
    sleep(1);
    cpu_main_loop();

exit:
    printf("Exiting...\n");
    timers_exit();
    graphics_exit();
    exit(1);

    //sould never get to here
}
