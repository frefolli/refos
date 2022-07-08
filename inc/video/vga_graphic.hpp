#ifndef __VIDEO_VGA_GRAPHIC_HPP__
#define __VIDEO_VGA_GRAPHIC_HPP__

namespace video {
    class VGAGraphicAdapter : public Adapter {
        private:
            screen_t properties;
            mode_t mode;
        public:
            VGAGraphicAdapter(screen_t properties);
            ~VGAGraphicAdapter();

            screen_t getVideoProperties() = 0;
            move_t   getVideoMode() = 0;

            void   printChar(char c) = 0;
            void   printInteger(uint64_t num, uint8_t base) = 0;
            void   printString(const char* str) = 0;
            void   clearScreen() = 0;
    }
}

#endif
