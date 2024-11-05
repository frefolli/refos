#!/bin/bash
BUILDDIR="${1:-.}"
ISO=${BUILDDIR}/refos.iso

qemu-system-x86_64 -machine q35 -m 256 -smp 2 -net none \
  -global driver=cfi.pflash01,property=secure,value=on \
  -drive if=pflash,format=raw,unit=0,file=/usr/share/edk2/ovmf/OVMF_CODE.fd,readonly=on \
  -drive if=ide,format=raw,media=cdrom,file=${ISO}
  #-drive if=pflash,format=raw,unit=1,file=/usr/share/edk2/ovmf/OVMF_VARS.fd \
