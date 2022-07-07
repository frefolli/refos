global start
global ADDRESS
global MAGIC

section .boot_point

bits 32

start:
    mov dword [ADDRESS], ebx
    mov dword [MAGIC], eax

    extern setup_paging
    jmp setup_paging

section .data

ADDRESS:
    dd 0
    dd 0

MAGIC:
    dd 0
    dd 0
