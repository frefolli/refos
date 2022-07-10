# RefOS

Handmade bare operative system.

## Build instructions

To build the `./build/os.iso` just call `make clean config compile`.

## Emulation instructions

To run the built iso, call either:

- `make bochs`, to call Bochs as Virtualizer

- `make run`, to call Qemu as Virtualizer

## State of Art

### Grub

![without EFI (= text mode available)](grub.png)

### Boot

here shutdown is stated as "kernel die", and infinite loop

![sample kernel state dump](boot.png)
