#include "legacy/stdbool.hpp"
#include "video/video.hpp"
#include "video/vga_graphic.hpp"
#include "video/vga_text.hpp"
#include "kernel/kernel.hpp"

video::Adapter::Adapter() {
    //
}

video::Adapter::~Adapter() {
    //
}

inline bool isTextMode(uint8_t type) {
    return (type == 2);
}

inline bool isGraphicMode(uint8_t type) {
    return (type != 2);
}

video::Adapter* video::Adapter::buildAdapter(screen_t properties) {
    if (isTextMode(properties.type)) {
        uint8_t* ptr = (uint8_t*)
            kernel::kernel.memory->malloc(sizeof(video::VGATextAdapter));
        video::VGATextAdapter *adapter = new (ptr) video::VGATextAdapter(properties);
        return (video::Adapter*) adapter;
    } else if (isTextMode(properties.type)) {
        uint8_t* ptr = (uint8_t*)
            kernel::kernel.memory->malloc(sizeof(video::VGAGraphicAdapter));
        video::VGAGraphicAdapter *adapter = new (ptr) video::VGAGraphicAdapter(properties);
        return (video::Adapter*) adapter;
    }
    return 0;
}
