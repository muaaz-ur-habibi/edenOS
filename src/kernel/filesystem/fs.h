#ifndef KFS
#define KFS

#include "../drivers/pci/pci.h"
#include "../drivers/vga/vga.h"

#define SATA_CONT 0x06
#define NVME_CONT 0x08
#define IDE_CONT 0x01

typedef struct
{
    uint32_t clb;
    uint32_t uclb;
    uint32_t fba;
    uint32_t ufba;

    uint32_t is;
    uint32_t ie;

    uint32_t cmd;
    uint32_t reserved;

    uint32_t task_file_data;
    uint32_t signature;

    uint32_t sata_status;
    uint32_t sata_control;
    uint32_t sata_error;
    uint32_t sata_active;
    uint32_t command_issue;

    uint32_t sata_notification;
    uint32_t fbs;

    uint32_t reserved2[11];
    uint32_t vendor[4];
} HBA_PORT;


typedef struct {
    uint32_t capabilities;
    uint32_t global_host_control;
    uint32_t interrupt_status;
    uint32_t ports_implimented;
    uint32_t version;
    uint32_t ccc_control;
    uint32_t ccc_ports;
    uint32_t em_location;
    uint32_t em_control;
    uint32_t capabilities_ex;
    uint32_t bios_handoff;

    uint8_t reserved[0x100 - 0x2c];

    HBA_PORT ports[32];
} HBA_MEM;

int find_storage_controller();
void init_sata_controller(pci_device_t sata);

#endif