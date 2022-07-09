#ifndef __VIDEO_VGA_GRAPHIC_HPP__
#define __VIDEO_VGA_GRAPHIC_HPP__

#include "video.hpp"

namespace video {
    class VGAGraphicAdapter : public Adapter {
        private:
            screen_t properties;
            mode_t mode;
        public:
            VGAGraphicAdapter(screen_t properties);
            ~VGAGraphicAdapter();

            screen_t getVideoProperties();
            mode_t   getVideoMode();

            void   printChar(char c);
            void   printInteger(int64_t num, uint8_t base);
            void   printString(const char* str);
            void   printf(const char* fmt ...);
            void   clearScreen();
    };
};

#endif
