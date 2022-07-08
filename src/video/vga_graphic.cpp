#include "video/vga_graphic.hpp"

video::VGAGraphicAdapter::VGAGraphicAdapter(video::screen_t properties) {
    this->properties = properties;
    this->mode = video::mode_t::TEXT;
}

video::VGAGraphicAdapter::~VGAGraphicAdapter() {
    // TODO
}

video::screen_t video::VGAGraphicAdapter::getVideoProperties() {
    return this->properties;
}

video::mode_t video::VGAGraphicAdapter::getVideoMode() {
    return this->mode;
}

void video::VGAGraphicAdapter::printChar(char /*c*/) {
    // TODO
}

void video::VGAGraphicAdapter::printInteger(uint64_t /*num*/, uint8_t /*base*/) {
    // TODO
}

void video::VGAGraphicAdapter::printString(const char* /*str*/) {
    // TODO
}

void video::VGAGraphicAdapter::clearScreen() {
    // TODO
}

void video::VGAGraphicAdapter::printf(const char* fmt ...) {
    va_list args;
    va_start(args, fmt);
    
    while(*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            switch(*fmt) {
                case 'd' : {
                    int num = va_arg(args, int);
                    this->printInteger(num, 10);
                }; break;
                case 'x' : {
                    int num = va_arg(args, int);
                    this->printInteger(num, 16);
                }; break;
                case 's' : {
                    const char* str = va_arg(args, const char*);
                    this->printString(str);
                }; break;
                default: break;
            }; fmt++;
        } else {
            this->printChar(*fmt);
            fmt++;
        }
    }

    va_end(args);
}
