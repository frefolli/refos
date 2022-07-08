#include "kernel/kernel.hpp"

/* 
 * this function checks that bootloader is multiboot2
 * compliant and that multiboot2 tags are loaded correcly,
 * then read this informations and starts the Kernel
 */
extern "C" int kmain(uint64_t magic, uint64_t address) {
    boot::info_t boot_info;
    boot_info.meta.magic = magic;
    boot_info.meta.address = address;

    // processing
    if (boot::checkMagic(magic)) {
        if (boot::checkAddress(address)) {
            boot::readHeader((uint8_t*) address, &boot_info);

            Kernel kernel::kernel (&boot_info);
            kernel::main();
        }
    }
    return 0;
}
