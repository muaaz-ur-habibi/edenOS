cd src/kernel
./build.sh
cd ../../

echo "Copying kernel.elf"
cp src/kernel/build/kernel.elf iso/boot/

echo "Building iso"
grub-mkrescue -o iso/run/edenos.iso iso/
echo "ISO built"

echo "Running"
powershell.exe -Command "qemu-system-x86_64 -cdrom $(wslpath -w iso/run/edenos.iso) -display sdl -drive id=disk,file=$(wslpath -w disk.img),if=none,format=raw -device ich9-ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0"