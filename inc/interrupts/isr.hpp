#ifndef __INTERRUPTS_ISR_HPP__
#define __INTERRUPTS_ISR_HPP__

#include "kernel/kernel.hpp"

typedef struct {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t err;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} registers_t;

extern "C" void isr_handler(int intNum, registers_t* regs);
extern "C" void irq_handler(int intNum, registers_t* regs);

typedef void (*isr_t)(registers_t*);

namespace interrupts {
    void registerHandler(uint8_t n, isr_t handler);
}

#endif
