#include "fs.h"


int find_storage_controller()
{
    for (size_t i = 0; i < n_pci_devices; i++)
    {
        pci_device_t d = pci_devices[i];

        
        switch (d.subclass)
        {
            case 0x01:
                break;

            case 0x06:
                init_sata_controller(d);
                break;

            case 0x08:
                break;
        }
        
        /*
        if (d.class == 0x01)
        {
            vga_prints("Mass Storage Controller found\n");

            if (d.subclass == 0x06)
            {
                vga_prints("AHCI SATA Controller\n");
                return i;
            } else
            if (d.subclass == 0x01)
            {
                vga_prints("IDE Controller\n");
                return i;
            } else
            if (d.subclass == 0x08)
            {
                vga_prints("NVMe Controller");
                return i;
            }
        }
        */
    }

    return 0;
}

void init_sata_controller(pci_device_t sata)
{
    vga_prints("Initialising SATA...\nAHCI BAR5: ");
    vga_prints(k_hextostr(sata.bar[5]));
    vga_prints("\n");

    HBA_MEM *abar = (HBA_MEM *)sata.bar[5];
    
    for (size_t i = 0; i < 32; i++)
    {
        if (abar->ports_implimented & (1 << i))
        {
            HBA_PORT *port = &abar->ports[i];
            vga_prints("Checking port ");
            vga_prints(k_inttostr(i));
            vga_prints(", Status: ");
            vga_prints(k_hextostr(port->sata_status));
            vga_prints(", Signature: ");
            vga_prints(k_hextostr(port->signature));
            vga_prints("\n");

            uint32_t status = port->sata_status;
            uint8_t det = status & 0xf;
            uint8_t ipm = (status >> 8) & 0xf;

            if (det == 3 && ipm == 1)
            {
                vga_prints("SATA drive on: ");
                vga_prints(k_inttostr(i));
                vga_prints("\n");
            }
            
        }
    }
}