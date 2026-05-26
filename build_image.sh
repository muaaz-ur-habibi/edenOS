echo "Creating image"
dd if=/dev/zero of=main.img bs=512 count=2880
mkfs.fat -F 12 main.img -n "EDENOS"

echo "Creating bootloader and 2nd stage loader"
cd src/boot
./build.sh # bootloader build script
cd ../../

echo "Copying bootloader"
dd if=src/boot/boot.bin of=main.img conv=notrunc

echo "Copying 2nd stage loader"
mcopy -i main.img src/boot/stage2.bin "::stage2.bin"

echo "Creating kernel"
cd src/kernel
#./build.sh # kernel build script
cd ../../

echo "Copying kernel"
#mcopy -i main.img src/kernel/kernel.bin "::kernel.bin"
mcopy -i main.img src/test_kernel/kernel.bin "::kernel.bin"

echo "Finished making image"
echo "Running..."

powershell.exe -Command "qemu-system-i386 -drive format=raw,file="$(wslpath -w main.img)",if=floppy"