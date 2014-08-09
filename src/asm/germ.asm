;------------------------------------------------------------------------------
; Name:
;   germ
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
;   This must be encoded in x86 assembly as nasmx64 will not resolve labels
;   if used as memory addresses.
;
[section .text]

%include "general.mac"

global _start

_start:
    nop                             ; keep gdb happy

    call        delta
delta:
    pop         ebp
    sub         ebp, delta          ; our start address

    ; print out the debug message
    jmp         short data
print:
    xor         eax, eax
    mov          al, 0x4            ; sys_write
    mov          bl, 0x1            ; stdout fd
    pop         ecx                 ; address of string
    mov          dl, str_len        ; length
    int         0x80

    ; now begin to search for enough free space
    xor         eax, eax            ; search for NULL
    mov         edi, ebp            ; start address
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
    mov         edi, ebp
    add         edi, germ_len
    mov         edi, germ_len
    rep         movsb

    jmp         exit
fail:
    ; there is not enough space to copy
    mov         ebx, 0xDEADBEEF
    mov         eax, 0x1
exit:
    xor         eax, eax

    ret


data:
    call        print
str:
    db          "Germ executing",10
str_len:        equ $-str

germ_len:       equ end-_start
end:
