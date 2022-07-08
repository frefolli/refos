#include "video/vga_text.hpp"

video::VGATextAdapter::VGATextAdapter(video::screen_t properties) {
    this->properties = properties;
    this->mode = video::mode_t::TEXT;
}

video::VGATextAdapter::~VGATextAdapter() {
    // TODO
}

video::screen_t video::VGATextAdapter::getVideoProperties() {
    return this->properties;
}

video::move_t video::VGATextAdapter::getVideoMode() {
    return this->mode;
}

void video::VGATextAdapter::printChar(char c) {
    switch(c) {
        case '\t' : {
            for (uint8_t i = 0; i < 4; i++)
                this->printChar(' ');
        }; break;
        
        case '\r' : {
            this->x = 0;
        }; break;
        
        case '\n' : {
            for (uint16_t i = this->x; i < this->properties->width)
                this->printChar(' ');
        }; break;
        
        default : {
            uint16_t* ptr = (uint16_t*) this->properties.framebuffer;
            uint32_t shift = this->x + this->y * this->properties.pitch;
            *(ptr + shift) = 
                (this->background << 12) | 
                (this->foreground << 8) | 
                (c);

            if (this->x == this->properties.width - 1) {
                this->x = 0;
                if (this->y == this->properties.height - 1) {
                    this->scroll();
                } else {
                    this->y++;
                }
            } else {
                this->x++;
            }
        }; break;
    }
}

void video::VGATextAdapter::printInteger(uint64_t num, uint8_t base) {
    // TODO
}

void video::VGATextAdapter::printString(const char* str) {
    // TODO
}

void video::VGATextAdapter::clearScreen() {
    uint16_t* ptr = (uint16_t*) this->properties.framebuffer;
    uint16_t* deadline = ptr + 
        (this->properties.height * this->properties.width);
    while(ptr < deadline) {
        *ptr = (this->background << 12) | 
               (this->foreground << 8) | 
               (' ');
    }

    this->x = 0;
    this->y = 0;
}

void video::VGATextAdapter::scrollScreen() {
    uint16_t* ptr = (uint16_t*) this->properties.framebuffer;
    uint16_t* deadline = ptr + 
        ((this->properties.height - 1) * this->properties.width);
    while(ptr < deadline) {
        *ptr = *(ptr + this->properties.width);
    }
}
