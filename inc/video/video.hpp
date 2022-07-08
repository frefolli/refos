#ifndef __VIDEO_HPP__
#define __VIDEO_HPP__

#include "../legacy/stdint.hpp"
#include "../legacy/stdarg.hpp"

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
    };

    class Adapter {
        public:
            Adapter(/* empty adapter */);
            ~Adapter();

            virtual screen_t getVideoProperties() = 0;
            virtual mode_t   getVideoMode() = 0;

            virtual void   printChar(char c) = 0;
            virtual void   printInteger(uint64_t num, uint8_t base) = 0;
            virtual void   printString(const char* str) = 0;
            virtual void   printf(const char* fmt ...) = 0;
            virtual void   clearScreen() = 0;

            static Adapter* buildAdapter(screen_t properties);
    };

};

#endif
