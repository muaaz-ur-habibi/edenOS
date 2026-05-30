#ifndef KERNEL
#define KERNEL

#include "drivers/vga/vga.h"
#include "drivers/graphics/graphics.h"
#include "utils/ui.h"
#include "drivers/port/port.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "interrupts/isr.h"
#include "irqs/timer.h"
#include "utils/memory.h"
#include "utils/string.h"

#define GRAPHICS_MODE_VGA 0x10
#define GRAPHICS_MODE_RGB 0x11

#define TARGET_FPS 60
#define FRAME_TIME_MS (1000/TARGET_FPS)

//uint32_t current_gfx_mode = 0;
void handle_user_inp(char **inp, int n_inputs);

#endif