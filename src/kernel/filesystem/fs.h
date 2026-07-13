#ifndef KFS
#define KFS

#include "../drivers/pci/pci.h"
#include "../drivers/vga/vga.h"

extern int ahci_found;

void find_ahci_controller();

#endif