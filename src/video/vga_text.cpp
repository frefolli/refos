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

video::mode_t video::VGATextAdapter::getVideoMode() {
    return this->mode;
}

inline void outb (unsigned short _port, unsigned char _data) {
    asm __volatile__ ("outb %0, %1" : : "a" (_data), "dN" (_port));
}

void video::VGATextAdapter::updateCursor() {
	uint16_t pos = this->x + this->y * this->properties.width;
 
    outb(0x03d4, 0x0f);
    outb(0x03d5, (pos & 0xff));
    outb(0x03d4, 0x0e);
    outb(0x03d5, ((pos >> 8) & 0xFF));
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
            for (uint16_t i = this->x; i < this->properties.width; i++)
                this->printChar(' ');
        }; break;
        
        default : {
            uint16_t* ptr = (uint16_t*) this->properties.framebuffer;
            uint64_t shift = this->x + this->y * this->properties.width;
            *(ptr + shift) = 
                (this->background << 12) | 
                (this->foreground << 8) | 
                (c);

            if (this->x == this->properties.width - 1) {
                this->x = 0;
                if (this->y == this->properties.height - 1) {
                    this->scrollScreen();
                } else {
                    this->y++;
                }
            } else {
                this->x++;
            }
        }; break;
    }
    this->updateCursor();
}

// max base 16
static const char* _digits = "0123456789abcdef";
void video::VGATextAdapter::printInteger(int64_t num, uint8_t base) {
    char buffer[21]; buffer[20] = '\0';
    char* ptr = buffer + 20;
    do {
        ptr--;
        *ptr = _digits[num % base];
        num /= base;
    } while(num != 0);

    if (num < 0)
        this->printChar('-');
    if (base == 16)
        this->printString("0x");
    this->printString(ptr);
}

void video::VGATextAdapter::printString(const char* str) {
    while(*str != '\0') {
        this->printChar(*str); str++;
    }
}

void video::VGATextAdapter::clearScreen() {
    uint16_t* ptr = (uint16_t*) this->properties.framebuffer;
    uint16_t* deadline = ptr + 
        (this->properties.height * this->properties.width);
    while(ptr < deadline) {
        *ptr = (this->background << 12) | 
               (this->foreground << 8) | 
               (' ');
        ptr++;
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
        ptr++;
    }
}

void video::VGATextAdapter::printf(const char* fmt ...) {
    va_list args;
    va_start(args, fmt);
    
    while(*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            switch(*fmt) {
                case 'd' : {
                    int64_t num = va_arg(args, int64_t);
                    this->printInteger(num, 10);
                }; break;
                case 'x' : {
                    int64_t num = va_arg(args, int64_t);
                    this->printInteger(num, 16);
                }; break;
                case 's' : {
                    const char* str = va_arg(args, const char*);
                    this->printString(str);
                }; break;
                default: this->printChar(*fmt);
            }; fmt++;
        } else {
            this->printChar(*fmt);
            fmt++;
        }
    }

    va_end(args);
}
