#define UHCI_USBCMD     0x00  // Command
#define UHCI_USBSTS     0x02  // Status
#define UHCI_USBINTR    0x04  // Interrupt Enable
#define UHCI_FRNUM      0x06  // Frame Number
#define UHCI_FRBASEADD  0x08  // Frame List Base Address (32-bit)
#define UHCI_SOFMOD     0x0C  // Start of Frame Modify
#define UHCI_PORTSC1    0x10  // Port 1 Status/Control
#define UHCI_PORTSC2    0x12  // Port 2 Status/Control

// USBCMD bits
#define CMD_RUN         (1 << 0)
#define CMD_HCRESET     (1 << 1)
#define CMD_GRESET      (1 << 2)
#define CMD_MAXP        (1 << 7)  // 1 = 64 byte packets

// PORTSC bits
#define PORT_CONNECT        (1 << 0)
#define PORT_CONNECT_CHANGE (1 << 1)
#define PORT_ENABLE         (1 << 2)
#define PORT_RESET          (1 << 9)