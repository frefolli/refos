#!/bin/bash
ISO=./builddir/refos.iso

function run-stick() {
  sudo qemu-system-x86_64 -machine q35 -m 256 -smp 2 -net none \
    -global driver=cfi.pflash01,property=secure,value=on \
    -drive if=pflash,format=raw,unit=0,file=/usr/share/edk2/ovmf/OVMF_CODE.fd,readonly=on \
    -drive if=pflash,format=raw,unit=1,file=/usr/share/edk2/ovmf/OVMF_VARS.fd \
    -drive if=ide,format=raw,file=$ISO
}

run-stick
