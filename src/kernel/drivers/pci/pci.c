#include "pci.h"

pci_device_t pci_devices[MAX_PCI_DEVICES];
size_t n_pci_devices = 0;

uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc)) | ((uint32_t)0x80000000);

    port_long_out(0xcf8, address);

    tmp = (uint16_t)((port_long_in(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)(bus << 16);
    uint32_t lslot = (uint32_t)(slot << 11);
    uint32_t lfunc = (uint32_t)(func << 8);

    address = 0x80000000 | lbus | lslot | lfunc | (offset & 0xfc);

    port_long_out(0xcf8, address);

    return port_long_in(0xcfc);
}

void pci_load_devices()
{
    for (size_t bus = 0; bus < 256; bus++)
    {
        for (size_t slot = 0; slot < 32; slot++)
        {
            for (size_t func = 0; func < 8; func++)
            {
                uint16_t vendor = pci_read_word(bus, slot, func, 0);
                if (vendor == 0xffff)
                {
                    continue;
                }
                if (n_pci_devices < MAX_PCI_DEVICES)
                {
                    pci_device_t *dev = &pci_devices[n_pci_devices++];
                    dev->vend_id = vendor;
                    dev->dev_id = pci_read_word(bus, slot, func, 2);
                    dev->class = (uint8_t)(pci_read_word(bus, slot, func, 0xa) >> 8);
                    dev->subclass = (uint8_t)(pci_read_word(bus, slot, func, 0xa) & 0xff);
                    dev->prog_if = (uint8_t)(pci_read_word(bus, slot, func, 0x08) >> 8);

                    for (size_t bar = 0; bar < 6; bar++)
                    {
                        dev->bar[bar] = pci_read_dword(bus, slot, func, 0x10 + (bar * 4));
                    }

                    vga_prints("\nFound device\n");
                    vga_prints(k_hextostr(dev->vend_id));
                    vga_prints(":");
                    vga_prints(k_hextostr(dev->dev_id));
                    vga_prints("\n");
                }
            }
        }
    }
}