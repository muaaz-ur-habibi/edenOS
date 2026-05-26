#ifndef KTERM
#define KTERM

#include <stdint.h>
#include <stddef.h>
#include "../port/port.h"
#include "../../utils/memory.h"
#include "../../utils/string.h"
#include "../../globals/types.h"

/*
    Basic terminal driver for simple IO
*/

#define VGA_MEMORY_ADDRESS 0xb8000
#define VGA_W 80
#define VGA_H 25

#define REGISTER_VGA_CTRL 0x3d4
#define REGISTER_VGA_DATA 0x3d5

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entrycolor(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
static inline uint16_t vga_entrychar(unsigned char ch, uint8_t color)
{
	return (uint16_t) ch | (uint16_t) color << 8;
}

void vga_clear();

void vga_putchar(char ch);
void vga_putchar_at(char ch, int row, int col);
void vga_remove_char(int n);

void vga_prints(char *str);
void vga_prints_at(char *str, int row, int col);

int get_cursor_offset();
void set_cursor_offset(int offset);

void vga_init();
void vga_set_color(int fg, int bg);

#endif