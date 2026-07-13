echo "Building Kernal ASM Files"
nasm -f elf32 kernel.asm -o build/kasm.o
nasm -f elf32 drivers/port/port.asm -o build/kport.o
nasm -f elf32 interrupts/isr.asm -o build/kisrasm.o

CFLAGS="-m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs -fno-stack-protector -mno-red-zone -O3"
echo "Building Kernel C Files"
gcc $CFLAGS -c kernel.c -o build/kc.o
gcc $CFLAGS -c drivers/vga/vga.c -o build/kvga.o
gcc $CFLAGS -c interrupts/isr.c -o build/kisr.o
gcc $CFLAGS -c interrupts/idt.c -o build/kidt.o
gcc $CFLAGS -c irqs/timer.c -o build/kirqtimer.o
gcc $CFLAGS -c drivers/keyboard/keyboard.c -o build/kkb.o
gcc $CFLAGS -c drivers/mouse/mouse.c -o build/kmouse.o
gcc $CFLAGS -c utils/memory.c -o build/kutilsmem.o
gcc $CFLAGS -c utils/string.c -o build/kutilstr.o
gcc $CFLAGS -c utils/ui.c -o build/kui.o
gcc $CFLAGS -c drivers/graphics/graphics.c -o build/kgraphics.o
gcc $CFLAGS -c drivers/pci/pci.c -o build/kpci.o
gcc $CFLAGS -c filesystem/fs.c -o build/kfs.o

echo "Linking kernel files..."
ld -m elf_i386 -T link.ld -o build/kernel.elf build/kasm.o build/kc.o build/kvga.o build/kfs.o build/kport.o build/kisrasm.o build/kisr.o build/kidt.o build/kirqtimer.o build/kpci.o build/kkb.o build/kutilsmem.o build/kutilstr.o build/kgraphics.o build/kui.o build/kmouse.o

echo "Finished building kernel.elf"

#echo "Running..."
# cant use this command with multiboot2 (grub version)
#powershell.exe -Command "qemu-system-i386 -kernel kernel.elf"