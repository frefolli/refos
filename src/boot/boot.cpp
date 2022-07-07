#include "boot/boot.hpp"
#include "boot/multiboot2.hpp"

inline int align_address(uint32_t reladdr) {
    int rem = reladdr % 8;
    if (rem == 0)
        return reladdr;
    else
        return reladdr + (8 - rem);
}

void read_multiboot2_tag_framebuffer(multiboot_tag_framebuffer_common* tag, boot_info_t* boot_info) {
    boot_info->screen.framebuffer = tag->framebuffer_addr;
    boot_info->screen.width = tag->framebuffer_width;
    boot_info->screen.height = tag->framebuffer_height;
    boot_info->screen.pitch = tag->framebuffer_pitch;
    boot_info->screen.bpp = tag->framebuffer_bpp;
    boot_info->screen.type = tag->framebuffer_type;
}

void read_multiboot2_header(uint8_t* address, boot_info_t* boot_info) {
    // read meta
    multiboot_basic_tag* basic_tag = (multiboot_basic_tag*) address;
    boot_info->meta.total_size = basic_tag->total_size;

    /*
    // big cycle for all tags
    address += 8;
    multiboot_tag* tag;
    for (
        tag = (multiboot_tag*) address;
        tag->type != MULTIBOOT_TAG_TYPE_END;
        address += align_address(tag->size)
    ) {
        if (tag->size == 0) {
            uint16_t* buff = (uint16_t*) 0xb8000;
            *(buff) = 0xff47;
        }
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER : {
                read_multiboot2_tag_framebuffer(
                        (multiboot_tag_framebuffer_common*) tag, boot_info);
                break;
            }
            default: break;
        }
    }
    */
};
