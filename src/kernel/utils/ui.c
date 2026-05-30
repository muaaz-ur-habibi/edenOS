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

void ui_draw_vline(uint32_t x, uint32_t y, uint32_t l, uint32_t color)
{
    for (size_t i = 0; i < l; i++)
    {
        gfx_putpixel(x, y+i, color);
    }
}

void ui_draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (1)
    {
        gfx_putpixel(x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
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

void ui_draw_triangle(uint32_t x, uint32_t y, uint32_t b, uint32_t h, uint32_t color)
{
    ui_draw_vline(x, y, h, color);
    ui_draw_hline(x, y+h, b, color);
    ui_draw_line((int)x, (int)y, (int)x+b, (int)y+h, color);
}

void ui_draw_cursor(uint32_t x, uint32_t y)
{
    //ui_draw_circle(x, y, 2, 0xff0000);
    ui_draw_triangle(x, y, 9, 12, 0xff0000);
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

void ui_draw_windows()
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