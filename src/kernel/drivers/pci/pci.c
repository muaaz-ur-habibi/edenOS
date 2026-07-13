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
                    uint16_t device = pci_read_word(bus, slot, func, 2);

                    pci_device_t *dev = &pci_devices[n_pci_devices++];
                    dev->vend_id = vendor;
                    dev->dev_id = device;

                    vga_prints("\nFound device\n");
                    vga_prints(k_hextostr(dev->vend_id));
                    vga_prints("\n");
                }
            }
        }
    }
}