#include "kernel/kernel.hpp"
#include "legacy/stdbool.hpp"

kernel::Kernel kernel::kernel;
/* 
 * this function checks that bootloader is multiboot2
 * compliant and that multiboot2 tags are loaded correcly,
 * then read this informations and starts the Kernel
 */
extern "C" int kmain(uint64_t magic, uint64_t address) {
    boot::info_t bootInfo;
    bootInfo.meta.magic = magic;
    bootInfo.meta.address = address;

    // processing
    if (boot::checkMagic(magic)) {
        if (boot::checkAddress(address)) {
            boot::readHeader((uint8_t*) address, &bootInfo);

            kernel::kernel = kernel::Kernel(&bootInfo);
            kernel::kernel.main();
        }
    }
    return 0;
}
