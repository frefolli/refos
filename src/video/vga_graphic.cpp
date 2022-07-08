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

video::move_t video::VGAGraphicAdapter::getVideoMode() {
    return this->mode;
}

void video::VGAGraphicAdapter::printChar(char c) {
    // TODO
}

void video::VGAGraphicAdapter::printInteger(uint64_t num, uint8_t base) {
    // TODO
}

void video::VGAGraphicAdapter::printString(const char* str) {
    // TODO
}

void video::VGAGraphicAdapter::clearScreen() {
    // TODO
}
