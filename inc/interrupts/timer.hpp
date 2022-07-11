#ifndef __INTERRUPTS_TIMER_HPP_
#define __INTERRUPTS_TIMER_HPP_

#include "legacy/stdint.hpp"

namespace interrupts {
    void initTimer(uint32_t frequency);
}

#endif
