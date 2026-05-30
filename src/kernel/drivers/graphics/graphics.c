#include "graphics.h"

framebuffer_t framebuffer;
uint32_t *backbuffer;
int font_char_width = 8, font_char_height = 16;

int should_render = 0;

void gfx_init(uint32_t *addr, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp)
{
    framebuffer.addr = addr;
    framebuffer.width = width;
    framebuffer.height = height;
    framebuffer.bpp = bpp;
    framebuffer.pitch = pitch;

    backbuffer = (uint32_t *)k_malloc(pitch * height);
    backbuffer = k_memset(backbuffer, pitch * height, 0);
}

void switch_buffers()
{
    k_memcpy(framebuffer.addr, backbuffer, framebuffer.pitch * framebuffer.height);
    //for (uint32_t i = 0; i < framebuffer.width * framebuffer.height; i++) { framebuffer.addr[i] = backbuffer[i]; }
}

void gfx_putpixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x >= framebuffer.width || y >= framebuffer.height)
    {
        return;
    }
    
    uint32_t bytesperpixel = framebuffer.bpp / 8;
    uint8_t *pixel;
    
    pixel = (uint8_t *)(backbuffer) + y*framebuffer.pitch + x*bytesperpixel;
    //pixel = (uint8_t *)(framebuffer.addr) + y*framebuffer.pitch + x*bytesperpixel;

    // accomodate for different bytesperpixel
    switch (framebuffer.bpp)
    {
    case 32:
        *(uint32_t *)pixel = color;
        break;
    case 24:
        pixel[0] = color & 0xff;
        pixel[1] = (color >> 8) & 0xff;
        pixel[2] = (color >> 16) & 0xff;
        break;
    case 16:
        *(uint16_t*)pixel = (uint16_t)color;
        break;
    case 8:
        *(uint8_t *)pixel = (uint8_t)color;
        break;
    }
}

void gfx_drawchar(int x, int y, char c, uint32_t color)
{
    int font_char_width = 8, char_height = 16;
    
    for (int row = 0; row < char_height; row++) {
        unsigned char byte = font[(unsigned char)c][row];
        for (int col = 0; col < font_char_width; col++) {
            if (byte & (1 << (7 - col))) {  // Pixel is set
                gfx_putpixel(x+col, y+row, color);
            }
        }
    }
}

void gfx_drawstring(int x, int y, char *string, uint32_t color)
{
    for (size_t i = 0; i < k_strlen(string); i++)
    {
        gfx_drawchar(x+(font_char_width*i), y, string[i], color);
    }
}

void gfx_cls(uint32_t color)
{
    for (uint32_t y = 0; y < framebuffer.height; y++)
    {
        for (uint32_t x = 0; x < framebuffer.width; x++)
        {
            gfx_putpixel(x, y, color);
        }
    }

    gfx_drawstring(20, 500, k_hextostr((int)backbuffer), 0xffffff);
}