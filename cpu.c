#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <time.h>

#include "include/mem.h"

#include "include/graphics.h"
#include "include/input.h"
#include "include/sound.h"
#include "include/timers.h"

//multiple regs
uint8_t regs[16]; // V registers
uint16_t reg_I;
uint8_t draw_screen[32][64] = {0};
//single
uint8_t reg_SP;
uint16_t reg_PC;
volatile uint8_t reg_delay_TIMER;
volatile uint8_t reg_sound_TIMER;
//stack
uint16_t stack[16];

//some interpreter specific things
bool skip_pc_inc;
bool cpu_exit;

void init_cpu() {
    memset(&regs, 0x00, sizeof(regs));
    reg_I = 0;

    reg_SP = 0xF;
    reg_PC = 0x200;
    reg_delay_TIMER = 0;

    memset(&stack, 0x00, sizeof(stack));

    //
    skip_pc_inc = false;
    cpu_exit = false;

    //initialize the random number generator
    srand((unsigned int) time(NULL));
}

void cpu_decode_execute_inst(uint16_t inst) {
    uint8_t reg_number;
    uint8_t reg_number_2;
    uint8_t temp_value;
    uint16_t temp_value_16;

    //used for drawing
    uint8_t reg_number_x;
    uint8_t reg_number_y;
    uint8_t disp_x;
    uint8_t disp_y;

    uint8_t curr_x;
    uint8_t curr_y;

    uint8_t disp_temp;

    switch (inst & 0xF000) {
        //case 0x00:
        //    break;

        case 0x0000:
            switch (inst & 0x00FF) {
                case 0x00E0:
                    graphics_clear_screen();
                    memset(&draw_screen, 0x00, sizeof(draw_screen));
                    break;

                case 0x00EE:
                    //ret

                    //check if sp is already empty (is at 15)
                    if (reg_SP == 0x0F) {
                        printf("\n\n\n\nFATAL: Atempted to return while the stack is empty!\n\n\n\n");
                        cpu_exit = true;
                        break;
                    }

                    skip_pc_inc = true;
                    reg_PC = stack[++reg_SP];
                    break;
            }
            break;

      case 0x1000:
            //set the pc value
            skip_pc_inc = true;
            reg_PC = (inst & 0x0FFF);
            break;

      case 0x2000:
            //call and jump to value

            //check if th sp is already zero
            if (reg_SP == 0x00) {
                printf("\n\n\n\nFATAL: Atempted to call while the stack is full!\n\n\n\n");
                cpu_exit = true;
                break;
            }

            skip_pc_inc = true;
            stack[reg_SP--] = reg_PC + 2;
            reg_PC = (inst & 0x0FFF);
            break;

        case 0x3000:
            //skip the next instruction if the register value x equals to the value
            reg_number = (inst&0x0F00)>>8;
            temp_value = (inst&0x00FF);
            if (regs[reg_number] == temp_value)
                reg_PC += 2;
            break;

        case 0x4000:
            //skip the next instruction if the register value x does not equals to the value
            reg_number = (inst&0x0F00)>>8;
            temp_value = (inst&0x00FF);

            if (regs[reg_number] != temp_value)
                reg_PC += 2;
            break;

        case 0x5000:
            //skip the next instruction if the register value x is equal to registr value y
            reg_number = (inst&0x0F00)>>8;
            reg_number_2 = (inst&0x00F0)>>4;
            if (regs[reg_number] == regs[reg_number_2])
                reg_PC += 2;
            break;

        case 0x6000:
            //set register value x to yy
            reg_number = (inst&0x0F00)>>8;
            temp_value = (inst&0x00FF);
            regs[reg_number] = temp_value;
            break;

        case 0x7000:
            //add register value x with yy
            reg_number = (inst&0x0F00)>>8;
            temp_value = (inst&0x00FF);
            regs[reg_number] += temp_value;
            break;

        case 0x8000:
            reg_number = (inst&0x0F00)>>8;
            reg_number_2 = (inst&0x00F0)>>4;
            switch (inst & 0x000F) {
                case 0x0000:
                    regs[reg_number] = regs[reg_number_2];
                    break;

                case 0x0001:
                    regs[reg_number] |= regs[reg_number_2];
                    break;

                case 0x0002:
                    regs[reg_number] &= regs[reg_number_2];
                    break;

                case 0x0003:
                    regs[reg_number] ^= regs[reg_number_2];
                    break;

                case 0x0004:
                    temp_value_16 = regs[reg_number] + regs[reg_number_2];
                    regs[reg_number] = (temp_value_16 & 0x00FF);

                    if (temp_value_16 > 0xFF)
                        regs[0xF] = 1;

                    break;

                case 0x0005:
                    if (regs[reg_number] > regs[reg_number_2])
                        regs[0xF] = 1;
                    else
                        regs[0xF] = 0;

                    temp_value_16 = regs[reg_number] - regs[reg_number_2];
                    regs[reg_number] = (temp_value_16 & 0x00FF);
                    break;

                case 0x0006:
                    regs[0xF] = (regs[reg_number]&0x01);
                    regs[reg_number] >>= 1;

                case 0x0007:
                    if (regs[reg_number_2] > regs[reg_number])
                        regs[0xF] = 1;
                    else
                        regs[0xF] = 0;

                    temp_value_16 = regs[reg_number_2] - regs[reg_number];
                    regs[reg_number] = (temp_value_16 & 0x00FF);
                    break;

                case 0x000E:
                    regs[0xF] = (regs[reg_number]&0x80);
                    regs[reg_number] <<= 1;
                    break;

                default:
                    printf("0x8, Unknown opcode 0x%04X\n", inst);
                    //exit(-1);
                    cpu_exit = true;
            }
            break;

        case 0x9000:
            //skip the next instruction if the register value x is not equal to registr value y
            reg_number = (inst&0x0F00)>>8;
            reg_number_2 = (inst&0x00F0)>>4;
            if (regs[reg_number] != regs[reg_number_2])
                reg_PC += 2;
            break;

        case 0xA000:
            //I = value
            temp_value_16 = (inst&0x0FFF);
            reg_I = temp_value_16;
            break;

        case 0xB000:
            //PC = value + V[0]
            reg_PC = (inst&0x0FFF) + regs[0];
            break;

        case 0xC000:
            //reg x = random value AND yy
            reg_number = (inst&0x0F00)>>8;
            temp_value = (inst&0x00FF);
            regs[reg_number] = (rand() & 0xFF) & temp_value;
            break;

        case 0xD000:
            //Draw
            reg_number_x = (inst&0x0F00)>>8;
            reg_number_y = (inst&0x00F0)>>4;
            temp_value = (inst&0x000F);

            disp_x = regs[reg_number_x];
            disp_y = regs[reg_number_y];

            for (uint32_t y=0; y<temp_value; y++) {
                disp_temp = mem_load(reg_I + y, 1);

                for (uint32_t x=0; x<8; x++) {
                    curr_x = (disp_x + x) & 0x3F;
                    curr_y = (disp_y + y) & 0x1F;

                    if (draw_screen[curr_y][curr_x] == 0x80)
                        regs[0xF] = 1;

                    draw_screen[curr_y][curr_x] ^= (disp_temp&0x80);

                    //graphics_draw_pixel(curr_x, curr_y, ( draw_screen[curr_y][curr_x] ? false : true));

                    disp_temp <<= 1;
                }
            }
#ifdef DEBUG
            printf("Will draw now!\n");
#endif
            graphics_draw_screen((uint8_t (*)[64]) draw_screen);
#ifdef DEBUG
            printf("finished drawing\n");
#endif
            break;

        case 0xE000:
            reg_number = (inst&0x0F00)>>8;
            //Keyboard
            switch (inst & 0x00FF) {
                case 0x009E:
                    if (keys_pressed & (1<<regs[reg_number])) {
                        reg_PC += 2;
                    }
                    break;

                case 0x00A1:
                    if (keys_pressed & (1<<regs[reg_number])) {
                        //do nothing
                    } else {
                        reg_PC += 2;
                    }
                    //cpu_exit = true;
                    break;

                default:
                    printf("0xE, Unknown opcode 0x%04X\n", inst);
                    //exit(-1);
                    cpu_exit = true;
                    break;

            }
            break;

        case 0xF000:
            reg_number = (inst&0x0F00)>>8;
            switch (inst & 0x00FF) {
                case 0x0007:
                    regs[reg_number] = reg_delay_TIMER;
                    break;

                case 0x000A:
#ifdef DEBUG
                    printf("Entering input wait loop\n");
#endif
                    while (keys_pressed == 0x0000 && is_input_thread_running == true) { do_millis_sleep(0, 100); }
#ifdef DEBUG
                    printf("Got out of wait loop\n");
#endif

                    if (keys_pressed & 0x0001)
                        regs[reg_number] = 0x00;
                    else if (keys_pressed & 0x0002)
                        regs[reg_number] = 0x01;
                    else if (keys_pressed & 0x0004)
                        regs[reg_number] = 0x02;
                    else if (keys_pressed & 0x0008)
                        regs[reg_number] = 0x03;
                    else if (keys_pressed & 0x0010)
                        regs[reg_number] = 0x04;
                    else if (keys_pressed & 0x0020)
                        regs[reg_number] = 0x05;
                    else if (keys_pressed & 0x0040)
                        regs[reg_number] = 0x06;
                    else if (keys_pressed & 0x0080)
                        regs[reg_number] = 0x07;
                    else if (keys_pressed & 0x0100)
                        regs[reg_number] = 0x08;
                    else if (keys_pressed & 0x0200)
                        regs[reg_number] = 0x09;
                    else if (keys_pressed & 0x0400)
                        regs[reg_number] = 0x0A;
                    else if (keys_pressed & 0x0800)
                        regs[reg_number] = 0x0B;
                    else if (keys_pressed & 0x1000)
                        regs[reg_number] = 0x0C;
                    else if (keys_pressed & 0x2000)
                        regs[reg_number] = 0x0D;
                    else if (keys_pressed & 0x4000)
                        regs[reg_number] = 0x0E;
                    else if (keys_pressed & 0x8000)
                        regs[reg_number] = 0x0F;

                    break;

                case 0x0015:
                    reg_delay_TIMER = regs[reg_number];
                    break;

                case 0x0018:
                    reg_sound_TIMER = regs[reg_number];
                    if (reg_sound_TIMER > 0)
                        is_sound_playing = true;
                    break;

                case 0x001E:
                    reg_I += regs[reg_number];
                    break;

                case 0x0029:
                    /*
                        number sprite data is stored at 0x0000
                        with each number size of 8 bytes (5 bytes + 3 padding)
                    */
                    //reg_I = (regs[reg_number] * 8) + (0x0000);
                    reg_I = (regs[reg_number] << 3) + (0x0000);
                    break;

                case 0x0033:
                    temp_value = regs[reg_number];

                    for (int32_t i=2; i>=0; i--) {
                        mem_store((reg_I + i), (temp_value%10), 1);
                        temp_value /= 10;
                    }

                    break;

                case 0x0055:
                    //cpu_exit = true;
                    for (int32_t i=0; i<=reg_number; i++) {
                        mem_store((reg_I + i), regs[i], 1);
                    }
                    break;

                case 0x0065:
                    for (int32_t i=0; i<=reg_number; i++) {
                        regs[i] = mem_load((reg_I + i), 1);
                    }
                    break;

                default:
                    printf("0xF, Unknown opcode 0x%04X\n", inst);
                    //exit(-1);
                    cpu_exit = true;
                    break;

            }
            break;
        default:
            printf("Unknown opcode 0x%04X\n", inst);
            //exit(-1);
            cpu_exit = true;
    }
}

void cpu_main_loop() {
    uint16_t curr_instruction = 0;

    while (is_input_thread_running == true && is_timers_thread_running == true) {
        skip_pc_inc = false;

        //fetch
        curr_instruction = mem_load(reg_PC, 2);
#ifdef DEBUG
        printf("--%04x--\n", curr_instruction);
#endif

        //decode and execute
        cpu_decode_execute_inst(curr_instruction);

        //go to next
        if (skip_pc_inc == false)
            reg_PC += 2;

#ifdef DEBUG
        printf("PC: 0x%04X\n", reg_PC);
        printf("--------\n");
#endif

        if (cpu_exit == true)
            break;
    }

}

