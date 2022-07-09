#include "boot/boot.hpp"
#include "boot/multiboot2.hpp"
#include "kernel/kernel.hpp"

bool boot::checkMagic(uint64_t magic) {
    return (magic == MULTIBOOT2_BOOTLOADER_MAGIC);
}

bool boot::checkAddress(uint64_t address) {
    return !(address & 7);
}

bool boot::checkKernelEnd(uint64_t kernelEnd, info_t* boot_info) {
    if (kernelEnd > boot_info->memory.base) {
        boot_info->memory.base = kernelEnd;
        return true;
    };
    return false;
}

inline int align_address(uint32_t reladdr) {
    int rem = reladdr % 8;
    if (rem == 0)
        return reladdr;
    else
        return reladdr + (8 - rem);
}

void boot::readFramebufferTag(uint8_t* address, boot::info_t* boot_info) {
    address += 8; // skip type and size
    
    boot_info->screen.framebuffer = *((uint64_t*) address);
    address += 8; // just loaded framebuffer

    boot_info->screen.pitch = *((uint32_t*) address); address += 4;
    boot_info->screen.width = *((uint32_t*) address); address += 4;
    boot_info->screen.height = *((uint32_t*) address); address += 4;
    boot_info->screen.bpp = *((uint8_t*) address); address++;
    boot_info->screen.type = *((uint8_t*) address); address++;
}

void boot::readMemoryMapTag(uint8_t* address, boot::info_t* boot_info) {
    uint32_t* ptr = (uint32_t*) address;
    /*uint32_t type = *ptr;*/ ptr++;
    uint32_t size = *ptr; ptr++;
    uint32_t entry_size = *ptr; ptr++;
    /*uint32_t entry_version = *ptr;*/ ptr++;
    uint32_t arity = (size - 16) / (entry_size);

    for(uint32_t i = 0; i < arity; i++) {
        uint64_t* bigptr = (uint64_t*) ptr;
        uint64_t location  = *bigptr; bigptr++; ptr += 2;
        uint64_t length    = *bigptr; bigptr++; ptr += 2;
        uint32_t entry_type = *ptr; ptr++;
        /*uint32_t reserved = *ptr;*/ ptr++;

        if (entry_type == 1) {
            boot_info->memory.base = location;
            boot_info->memory.limit = length;
        }
    }
}

void boot::readTag(uint8_t* address, uint32_t* type, uint32_t* size) {
    uint32_t* ptr = (uint32_t*) address;
    *type = *ptr; ptr++;
    *size = *ptr;
}

void boot::readHeader(uint8_t* address, boot::info_t* boot_info) {
    boot_info->memory.mode = memory::mode_t::PAGED;

    // read meta
    multiboot_basic_tag* basic_tag = (multiboot_basic_tag*) address;
    boot_info->meta.total_size = basic_tag->total_size;
    uint8_t* deadline = (address + boot_info->meta.total_size);

    address += 8;
    uint32_t type, size;
    do {
        boot::readTag(address, &type, &size);
        
        switch (type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER : {
                boot::readFramebufferTag(address, boot_info); break;
            }
            case 6 : {
                boot::readMemoryMapTag(address, boot_info); break;
            }
            case MULTIBOOT_TAG_TYPE_END : break;
            default: break;
        }
        
        address += align_address(size);
    } while(address < deadline);
}
