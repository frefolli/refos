#include "legacy/stdbool.hpp"
#include "video/video.hpp"
#include "video/vga_graphic.hpp"
#include "video/vga_text.hpp"

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
        return new video::VGATextAdapter(properties);
    } else if (isTextMode(properties.type)) {
        return new video::VGAGraphicAdapter(properties);
    }
}
