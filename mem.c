#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "include/mem.h"

uint8_t mem_array[0x1000];

void init_mem() {
    memset(&mem_array, 0x00, sizeof(mem_array));
}

 void mem_store(uint16_t addr, uint16_t data, uint8_t len) {
    if (len == 0 || len > 2) {
        printf("Error: length is not 1 or 2, got %02X\n", len);
        exit(-1);
    }

    if (addr == 0xFFF && len == 2) {
        printf("Tried to write to 0xFFF with 2 bytes!!!\n", len);
        exit(-1);
    }

    if (addr > 0xFFF) {
        printf("Tried to write from 0x%04X, but it is not existant!!!\n", addr);
        exit(-1);
    }

    if (len == 1)
        mem_array[addr] = (data&0xFF);
    else {
        mem_array[addr] = (data&0xFF00)>>8;
        mem_array[addr+1] = (data&0xFF);
    }
        
}

uint16_t mem_load(uint16_t addr, uint8_t len) {
    uint16_t ret_val = 0;

    if (len == 0 || len > 2) {
        printf("Error: length is not 1 or 2, got %02X\n", len);
        exit(-1);
    }

    if (addr == 0xFFF && len == 2) {
        printf("Tried to read to 0xFFF with 2 bytes!!!\n", len);
        exit(-1);
    }

    if (addr > 0xFFF) {
        printf("Tried to read from 0x%04X, but it is not existant!!!\n", addr);
        exit(-1);
    }

    if (len == 1)
        ret_val = mem_array[addr];
    else {
        ret_val = mem_array[addr]<<8;
        ret_val |= mem_array[addr+1];
    }

    return ret_val;
}




