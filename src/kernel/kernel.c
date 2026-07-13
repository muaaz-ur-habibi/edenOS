#include "kernel.h"
#include "multiboot2.h"

void kernel_init() {
    k_heap_init();
    
    set_cursor_offset(0);
    isr_install_handlers();
    init_timer(1000);
    ui_init_windows();
    init_mouse();
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
    } else
    if (k_strcmp(inp[0], "pci"))
    {
        vga_prints("\nPci devices: ");
        vga_prints(k_inttostr(n_pci_devices));
        vga_prints("\n");      
        vga_prints("\nPci device at ");
        vga_prints(inp[1]);
        vga_prints("\n");
        pci_device_t d = pci_devices[k_strtoint(inp[1])];
        vga_prints("Vendor ");
        vga_prints(k_hextostr(d.vend_id));
        vga_prints(", Device ");
        vga_prints(k_hextostr(d.dev_id));
        vga_prints(", Class ");
        vga_prints(k_hextostr(d.class));
        vga_prints(", Subclass ");
        vga_prints(k_hextostr(d.subclass));
        vga_prints(", Prog_if");
        vga_prints(k_hextostr(d.prog_if));
        vga_prints("\n");
    }
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
				gfx_init(
                    (uint32_t *)fb,
                    fb_tag->common.framebuffer_width,
                    fb_tag->common.framebuffer_height,
                    fb_tag->common.framebuffer_pitch,
                    fb_tag->common.framebuffer_bpp
                );
				gfx_cls(BACKGROUND_COLOR);
                switch_buffers();
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

	ui_create_window(30, 50, 300, 200, 2, 0xffffff, "Console");

    int mousex, mousey;
    pci_load_devices();
    find_storage_controller();

    uint32_t frame_start = 0, frame_time = 0;
    while (1) {
        frame_time = get_tick() - frame_start;

        if ((get_tick() - frame_start) >= FRAME_TIME_MS)
        {
            frame_start = get_tick();
            gfx_cls(BACKGROUND_COLOR);

            // drawing the windows
            ui_draw_windows();
            ui_draw_widgets();

            gfx_drawstring(10, 10, k_inttostr((int)frame_time), 0);
            gfx_drawstring(10, 26, k_hextostr(pci_devices[0].vend_id), 0);

            // polling the mouse
            mouse_poll(&mousex, &mousey);
            ui_draw_cursor(mousex, mousey);

            switch_buffers();
        }
    }
}