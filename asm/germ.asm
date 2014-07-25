;------------------------------------------------------------------------------
; Name:
;   germ
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
[section .text]

global _start

_start:
    nop                             ; keep gdb happy
    call        store_eip
store_eip:
    pop         r15
    sub         r15, store_eip      ; our start address

    jmp         short data
code:
    ; print out the debug message
    xor         rax, rax
    mov          al, 0x1            ; sys_write
    mov         rdi, 0x1            ; stdout fd
    pop         rsi                 ; address of string
    mov         rdx, str_len        ; length
    syscall

    ; now begin to search for enough free space
    xor         rax, rax            ; search for NULL
    mov         rdi, r15            ; start address
    add         rdi, germ_len       ; start address + length
    xor         rcx, rcx
    add         rcx, germ_len       ; counter

loop:
    add         rdi, 0x1
    cmp         [rdi], al
    jne         fail

    dec         cl
    cmp         rcx, 0x0
    jne         loop

    ; there is space to copy
    mov         rdi, r15
    add         rdi, germ_len
    mov         rdi, germ_len
    rep         movsb

    jmp         exit
fail:
    ; there is not enough space to copy
    mov         rbx, 0xDEADBEEF
    mov         rax, 0x1
exit:
    xor         rax, rax
    ret


data:
    call        code
str:
    db          "Germ executing",10
str_len:        equ $-str

germ_len:       equ end-_start
end:
