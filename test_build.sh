cd src/boot
nasm -f bin boot.asm -o boot.bin
nasm -f bin 2nd_stage.asm -o stage2.bin

cd ../..

cat src/boot/boot.bin src/boot/stage2.bin > test_image.img

powershell.exe -Command "qemu-system-i386 -fda "$(wslpath -w test_image.img)""