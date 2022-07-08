#ifndef __VIDEO_HPP__
#define __VIDEO_HPP__

#include "../libc/stdint.hpp"

namespace video {

    typedef struct __attribute__((packed)) {
        uint64_t framebuffer;
        uint32_t width;
        uint32_t height;
        uint32_t pitch;
        uint8_t  bpp;
        uint8_t  type;
    } screen_t;

    enum mode_t {
        TEXT = 0,
        GRAPHIC = 1
    }

    class Adapter {
        public:
            Adapter(screen_t* properties);
            virtual ~Adapter();

            virtual screen_t getVideoProperties();
            virtual move_t   getVideoMode();

            virtual void   printChar(char c);
            virtual void   printInteger(uint64_t num, uint8_t base);
            virtual void   printString(const char* str);
            virtual void   clearScreen();

            static Adapter* buildAdapter(screen_t properties);
    }

}

#endif
