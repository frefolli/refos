global _kernel_entry
bits 64

section .text

extern MAGIC
extern ADDRESS

_kernel_entry:
    mov rdi, [MAGIC]
    mov rsi, [ADDRESS]
    extern kmain
    call kmain
