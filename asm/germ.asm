;------------------------------------------------------------------------------
; Name:
;   germ
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
[section .text]

%include "sys_calls.mac"            ; include my syscall macros

global _start

_start:
    call        store_eip
store_eip:
    pop         esi
    sub         esi, store_eip      ; our start address

    jmp         short data
code:

    pop         ecx                 ; address of string
    sys_write   1, ecx, str_len     ; fd, buff, len

    ; now begin to search for enough free space
    xor         eax, eax            ; search for NULL
    mov         edi, esi            ; start address
    add         edi, germ_len       ; start address + length
    xor         ecx, ecx
    add         ecx, germ_len       ; counter

loop:
    add         edi, 0x1
    cmp         [edi], al
    jne         fail

    dec         cl
    cmp         ecx, 0x0
    jne         loop

    ; there is space to copy
    mov         edi, esi
    add         edi, germ_len
    mov         ecx, germ_len
    rep         movsb

    jmp         exit
fail:
    ; there is not enough space to copy
    mov         ebx, 0xDEADBEEF
exit:
    ret


data:
    call        code
str:
    db          "Germ executing",10
str_len:        equ $-str

germ_len:       equ end-_start
end:
