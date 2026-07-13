#include "fs.h"

int ahci_found = 0;

void find_ahci_controller()
{
    for (size_t i = 0; i < n_pci_devices; i++)
    {
        pci_device_t d = pci_devices[i];
        
        if (d.class == 0x01)
        {
            vga_prints("Mass Storage Controller found\n");
            switch (d.subclass)
            {
                case 0x01:
                    vga_prints("IDE Controller\n");
                    break;

                case 0x06:
                    vga_prints("AHCI SATA Controller\n");
                    break;

                case 0x08:
                    vga_prints("NVMe Controller\n");
                    break;
            }
        }
    }
}