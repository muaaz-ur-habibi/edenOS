#ifndef KPCI
#define KPCI

#include "../port/port.h"
#include <stddef.h>
#include "../../drivers/vga/vga.h"

typedef struct
{
    uint8_t bus,slot,func;
    uint8_t class,subclass,prog_if;
    uint16_t dev_id, vend_id;
    uint8_t header_type;

    uint32_t bar[6];
} pci_device_t;

#define MAX_PCI_DEVICES 256
extern pci_device_t pci_devices[MAX_PCI_DEVICES];
extern size_t n_pci_devices;

uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_load_devices();

#endif