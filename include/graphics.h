#ifndef __term_h__
#define __term_h__

void graphics_init();
void graphics_exit();
void graphics_clear_screen();
//void graphics_draw_pixel(uint8_t x, uint8_t y, bool is_black);
void graphics_draw_screen(uint8_t screen[32][64]);

#endif
