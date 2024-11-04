#!/bin/bash
set -e
function compile-with-gcc() {
  gcc -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 -o ./builddir/main64.o -c src/main.c
  gcc -nostdlib -shared -Wl,-T,./scripts/x86_64.lds -Wl,-Bsymbolic -Wl,-znocombreloc -o ./builddir/kernel_x64.elf ./builddir/main64.o -lgcc
  objcopy -I elf64-x86-64 -O efi-app-x86_64 ./builddir/kernel_x64.elf ./builddir/BOOTX64.EFI
}

function create-fat() {
  dd if=/dev/zero of=./builddir/fat.img bs=1k count=1440
  mformat -i ./builddir/fat.img -f 1440 ::
  mmd -i ./builddir/fat.img ::/EFI
  mmd -i ./builddir/fat.img ::/EFI/BOOT
  mcopy -i ./builddir/fat.img ./builddir/BOOT*.EFI ::/EFI/BOOT
}

function run-stick() {
  sudo qemu-system-x86_64 -machine q35 -m 256 -smp 2 -net none \
    -global driver=cfi.pflash01,property=secure,value=on \
    -drive if=pflash,format=raw,unit=0,file=/usr/share/edk2/ovmf/OVMF_CODE.fd,readonly=on \
    -drive if=pflash,format=raw,unit=1,file=/usr/share/edk2/ovmf/OVMF_VARS.fd \
    -drive if=ide,format=raw,file=./builddir/fat.img
}

rm -rf ./builddir
mkdir -p ./builddir
compile-with-gcc
create-fat
run-stick
