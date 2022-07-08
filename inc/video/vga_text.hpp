#ifndef __VIDEO_VGA_TEXT_HPP__
#define __VIDEO_VGA_TEXT_HPP__

#include "video.hpp"

namespace video {
    enum color16_t {
        black = 0,
        blue = 1,
        green = 2,
        cyan = 3,
        red = 4,
        magenta = 5,
        brown = 6,
        white = 7,
        gray = 8,
        lightBlue = 9,
        lightGreen = 10,
        lightCyan = 11,
        lightRed = 12,
        lightMagenta = 13,
        yellow = 14,
        brightWhite = 15    
    }

    class VGATextAdapter : public Adapter {
        private:
            screen_t properties;
            mode_t mode;

            uint32_t cursor_x = 0;
            uint32_t cursor_y = 0;
            uint8_t foreground = color16_t::white;
            uint8_t background = color16_t::black;
        public:
            VGATextAdapter(screen_t properties);
            ~VGATextAdapter();

            screen_t getVideoProperties();
            move_t   getVideoMode();

            void   printChar(char c);
            void   printInteger(uint64_t num, uint8_t base);
            void   printString(const char* str);
            void   clearScreen();
            void   scrollScreen();
    }
}

#endif
