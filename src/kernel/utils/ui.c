#include "ui.h"

window_t windows[MAX_WINDOWS];

int free_ptr = 0;
int n_windows = 0;

void ui_init_windows()
{
    for (size_t i = 0; i < MAX_WINDOWS; i++)
    {
        windows[i].id = 0;
    }
}

void ui_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    for (uint32_t yy = y; yy < y+h; yy++)
    {
        for (uint32_t xx = x; xx < x+w; xx++)
        {
            gfx_putpixel(xx, yy, color);
        }
    }
}

void ui_draw_hline(uint32_t x, uint32_t y, uint32_t l, uint32_t color)
{
    for (size_t i = 0; i < l; i++)
    {
        gfx_putpixel(x+i, y, color);
    }
}

void ui_draw_circle(uint32_t xc, uint32_t yc, uint32_t r, uint32_t color)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x)
    {
        ui_draw_hline(xc - x, yc - y, x * 2 + 1, color);
        ui_draw_hline(xc - x, yc + y, x * 2 + 1, color);

        ui_draw_hline(xc - y, yc - x, y * 2 + 1, color);
        ui_draw_hline(xc - y, yc + x, y * 2 + 1, color);

        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}

void ui_draw_cursor(uint32_t x, uint32_t y)
{
    ui_draw_circle(x, y, 1, 0xffffff);
}

void ui_draw_button(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    ui_draw_rect(x, y, w, h, color);
}

void ui_draw_topbar(uint32_t x, uint32_t y, uint32_t w, char *title)
{
    uint32_t topbar_height = 20;
    y -= topbar_height;

    ui_draw_rect(x, y, w, topbar_height, 0x999999);
    ui_draw_button((x+w-40), y, 40, topbar_height, 0xff0000);
    gfx_drawstring(x, y, title, 0x000000);
}

void ui_create_window(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t border_size, uint32_t bg_color, char *title)
{
    window_t win;
    win.bg_color = bg_color;
    win.x = x;
    win.y = y;
    win.w = w;
    win.h = h;
    win.title = title;
    win.border_size = border_size;
    win.id = ((n_windows) % MAX_WINDOWS) + 1;

    windows[n_windows++] = win;
}

void ui_draw_window()
{
    for (size_t i = 0; i < n_windows; i++)
    {
        if (windows[i].id)
        {
            ui_draw_rect(
                windows[i].x-windows[i].border_size,
                windows[i].y-windows[i].border_size,
                windows[i].w+windows[i].border_size*2,
                windows[i].h+windows[i].border_size*2, 0x000000);
            ui_draw_topbar(
                windows[i].x,
                windows[i].y,
                windows[i].w, windows[i].title);
            ui_draw_rect(
                windows[i].x,
                windows[i].y,
                windows[i].w,
                windows[i].h, windows[i].bg_color);
        }
    }
}