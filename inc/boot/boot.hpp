#ifndef __BOOT_HPP__
#define __BOOT_HPP__

#include "../video/video.hpp"

typedef struct __attribute__((packed)) {
    uint32_t total_size;
} boot_info_meta_t;

typedef struct __attribute__((packed)) {
    boot_info_meta_t meta;
    screen_t screen;
} boot_info_t;

void read_multiboot2_header(uint8_t* address, boot_info_t* boot_info);

#endif
