#ifndef KUI
#define KUI

#include "../drivers/graphics/graphics.h"

#define MAX_WINDOWS 200

enum WIDGET_TYPE {
    WT_BUTTON,
};

typedef struct
{
    uint32_t x, y, w, h, border_size;
    uint32_t bg_color;
    char *title;
    int id;
    int focused;
} window_t;

typedef struct
{
    int type;
    uint32_t px, py;
    uint32_t w, h;
    uint32_t bg_color;
    int clickable;
} widget_t;

void ui_init_windows();
void ui_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void ui_draw_circle(uint32_t x, uint32_t y, uint32_t r, uint32_t color);
void ui_draw_triangle(uint32_t x, uint32_t y, uint32_t b, uint32_t h, uint32_t color);
void ui_draw_hline(uint32_t x, uint32_t y, uint32_t l, uint32_t color);
void ui_draw_vline(uint32_t x, uint32_t y, uint32_t l, uint32_t color);
void ui_draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void ui_draw_cursor(uint32_t x, uint32_t y);
void ui_create_window(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t border_size, uint32_t bg_color, char *title);
void ui_draw_topbar(uint32_t x, uint32_t y, uint32_t w, char *title);
void ui_draw_windows();

#endif