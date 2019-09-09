#ifndef __mem_h__
#define __mem_h__

extern uint8_t mem_array[];

void mem_store(uint16_t addr, uint16_t data, uint8_t len);
uint16_t mem_load(uint16_t addr, uint8_t len);
void init_mem();

#endif
