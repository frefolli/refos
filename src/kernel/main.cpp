#include "boot/boot.hpp"
#include "libc/stdint.hpp"

enum color_t {
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
};

typedef struct {
    uint16_t* buffer;
    uint32_t x;
    uint32_t y;
    color_t background;
    color_t foreground;
} vga_state_t;
 
static vga_state_t VGA_STATE = {
    (uint16_t*) 0xB8000,
    0, 0,
    black,
    white
};

void print_char(char c) {
    if (c == '\n') {
        VGA_STATE.x = 0;
        VGA_STATE.y ++;
    } else {
        *(VGA_STATE.buffer + VGA_STATE.y * 160 + VGA_STATE.x) = (VGA_STATE.background << 12) | (VGA_STATE.foreground << 8) | c;
        if (VGA_STATE.x == 79) {
            VGA_STATE.x = 0;
            VGA_STATE.y ++;
        } else
            VGA_STATE.x ++;
    }
}

void print_string(const char* msg) {
    while(*msg != '\0') {
        print_char(*msg);
        msg++;
    }
}

int integer_size(uint64_t num, uint8_t base = 10) {
    int size = 0;
    do {
        num /= base;
        size++;
    } while(num != 0);
    return size;
}

// till 16
static const char* base_chars = "0123456789abcdef";
void print_integer(uint64_t num, uint8_t base = 0) {
    if (num < base) {
        print_char(base_chars[num]);
    } else {
        print_integer(num / base, base);
        print_integer(num % base, base);
    }
}

extern void print_couple(uint64_t A, uint64_t B) {
    print_string("(0x");
    print_integer(A, 16);
    print_string(", 0x");
    print_integer(B, 16);
    print_string("); ");
}

extern "C" int kmain(uint64_t magic, uint64_t address) {
    print_couple(magic, address);
    // processing
    if (magic == 0x36d76289) {
        if (!(address & 7)) {
            boot_info_t boot_info;
            read_multiboot2_header(
                    (uint8_t*) address, &boot_info);
            
            print_couple(boot_info.screen.width, boot_info.screen.height);

            print_string("meta = (");
            print_integer(boot_info.meta.total_size, 10);
            print_string(");\n");

            while(true);
        }
    }
    return 0;
}
