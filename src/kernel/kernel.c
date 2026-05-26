#include "kernel.h"
#include "multiboot2.h"

uint8_t mouse_cycle = 0;
uint8_t mouse_bytes[3];
int mouse_x, mouse_y, mouseDown = 0;

void kernel_init() {
    set_cursor_offset(0);
    isr_install_handlers();
    init_timer(50);
    init_keyboard();
    asm volatile("sti");
}

// this only works for vga mode shell
void handle_user_inp(char **inp, int n_inputs)
{
    if (k_strcmp(inp[0], "quit"))
    {
        vga_prints("\nSee ya!\n");
        asm volatile("hlt");
    } else
    if (k_strcmp(inp[0], "help"))
    {
        vga_prints(
            "\n\nEdenOS Kernel, version 0.0\n"
            "   Commands:\n"
            "       help: display this help message\n"
            "       malloc: allocate n bytes\n"
            "       clear: clear the screen\n"
            "       quit: exit the kernel\n"
        );
    } else
    if (k_strcmp(inp[0], "setcol"))
    {
        vga_set_color(k_strtoint(inp[1]), k_strtoint(inp[2]));
    } else
    if (k_strcmp(inp[0], "clear"))
    {
        vga_clear();
    } else
    if (k_strcmp(inp[0], "malloc"))
    {
        if (!k_strcmp(inp[1], "") && n_inputs > 1)
        {
            k_malloc(k_strtoint(inp[1]));
            vga_prints("\nMemory allocated. Current pointer: "); vga_prints(k_inttostr(get_mem_idx())); vga_prints("\n");
        } else {
            vga_prints("\nmalloc usage: malloc [size to allocate]\n");
        }
    }
}

void mouse_write(uint8_t write_byte) {
    while ((port_byte_in(0x64) & 0x02));
    port_byte_out(0x64, 0xD4);
    while ((port_byte_in(0x64) & 0x02));
    port_byte_out(0x60, write_byte);
    while (!(port_byte_in(0x64) & 0x01));
    port_byte_in(0x60); 
}

void k_main(uint32_t magic, uint32_t mb2_addr) {
    kernel_init();
    char head[] = "EdenOS Version 0.1\n";
    vga_prints_at(head, 0, (VGA_W-k_strlen(head))/2);
    
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        vga_prints("not a valid multiboot2 bootloader ");
        vga_prints(k_hextostr(magic));
        vga_prints("\n");
    }

    /**/
    struct multiboot_tag *tag;
    unsigned size;
    for (tag = (struct multiboot_tag *) (mb2_addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
        {
            multiboot_uint32_t color;
            struct multiboot_tag_framebuffer *fb_tag = (struct multiboot_tag_framebuffer *)tag;
            void *fb = (void *)(unsigned long)fb_tag->common.framebuffer_addr;

            switch (fb_tag->common.framebuffer_type)
            {
            case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:

                break;
            case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
				gfx_init((uint32_t *)fb, fb_tag->common.framebuffer_width, fb_tag->common.framebuffer_height, fb_tag->common.framebuffer_pitch, fb_tag->common.framebuffer_bpp);
				gfx_cls(0xffff00);
				//current_gfx_mode = GRAPHICS_MODE_RGB;
                break;
            case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                vga_init();
                vga_prints(">> ");
				//current_gfx_mode = GRAPHICS_MODE_VGA;
                break;
            default:
				return;
                break;
            }

            for (size_t i = 0; i < fb_tag->common.framebuffer_width
                   && i < fb_tag->common.framebuffer_height; i++)
              {
                switch (fb_tag->common.framebuffer_bpp)
                  {
                  case 8:
                    {
                      multiboot_uint8_t *pixel = fb
                        + fb_tag->common.framebuffer_pitch * i + i;
                      *pixel = color;
                    }
                    break;
                  case 15:
                  case 16:
                    {
                      multiboot_uint16_t *pixel
                        = fb + fb_tag->common.framebuffer_pitch * i + 2 * i;
                      *pixel = color;
                    }
                    break;
                  case 24:
                    {
                      multiboot_uint32_t *pixel
                        = fb + fb_tag->common.framebuffer_pitch * i + 3 * i;
                      *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
                    }
                    break;

                  case 32:
                    {
                      multiboot_uint32_t *pixel
                        = fb + fb_tag->common.framebuffer_pitch * i + 4 * i;
                      *pixel = color;
                    }
                    break;
                  }
              }
        }
        
    }

    ui_init_windows();

    while ((port_byte_in(0x64) & 0x02));
    port_byte_out(0x64, 0xA8); 

    mouse_write(0xF6);
    mouse_write(0xF3);
    mouse_write(200);
    mouse_write(0xF4);

	ui_create_window(200, 100, 400, 300, 5, 0xffffff, "Console");

    while (1) {
        ui_draw_window();

        uint8_t stat = port_byte_in(0x64);
        
        if ((stat & 0x01))
        {
            if (stat & 0x20)
            {
                while (port_byte_in(0x64) & 0x01)
                {
                    uint8_t data = port_byte_in(0x60);
                    if (mouse_cycle == 0) {
                        if (!(data & 0x08)) continue; 
                        mouse_bytes[0] = data;
                        mouse_cycle = 1;
                    } 
                    else if (mouse_cycle == 1) {
                        mouse_bytes[1] = data;
                        mouse_cycle = 2;
                    } 
                    else if (mouse_cycle == 2) {
                        mouse_bytes[2] = data;
                        mouse_cycle = 0; 

                        uint8_t flags = mouse_bytes[0];
                        int16_t rel_x = mouse_bytes[1];
                        int16_t rel_y = mouse_bytes[2];

                        if (flags & 0x10) rel_x |= 0xFF00; 
                        if (flags & 0x20) rel_y |= 0xFF00;

                        int16_t abs_x = (rel_x < 0) ? -rel_x : rel_x;
                        int16_t abs_y = (rel_y < 0) ? -rel_y : rel_y;
                        if (abs_x > 5) rel_x *= 2; 
                        if (abs_y > 5) rel_y *= 2;

                        mouse_x += rel_x;
                        mouse_y -= rel_y; 

                        // 0 = L, 1 = R, 2 = M
                        mouseDown = (flags & 0x01); 

                        if (mouse_x < 0) mouse_x = 0;
                        if (mouse_y < 0) mouse_y = 0;
                        if (mouse_x >= 1024)  mouse_x = 1024 - 1;
                        if (mouse_y >= 768) mouse_y = 768 - 1;
                    }
                }
                
                ui_draw_cursor(mouse_x, mouse_y);
            }
        }
    }
}