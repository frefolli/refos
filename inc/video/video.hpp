#ifndef __VIDEO_HPP__
#define __VIDEO_HPP__

#include "../libc/stdint.hpp"

typedef struct __attribute__((packed)) {
    uint64_t framebuffer;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t  bpp;
    uint8_t  type;
} screen_t;

#endif
