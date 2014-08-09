;------------------------------------------------------------------------------
; Name:
;   germ
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
;   This must be encoded in x86 assembly as nasm x64 will not resolve labels
;   if used as memory addresses.
;
[section .text]

global _start

_start:
    nop
    call        delta
delta:
    pop         ebp
    sub         ebp, delta          ; our start address

    ; print out the begin execution message
    lea         ecx, [ebp + exe_str]
    mov         edx, exe_str_len
    call        print

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

    ; print failure message
    lea         ecx, [ebp + fail_str]
    mov         edx, fail_str_len
    call        print

    mov         ebx, 0xDEADBEEF
    mov         eax, 0x1
exit:

    ; print completion message
    lea         ecx, [ebp + comp_str]
    mov         edx, comp_str_len
    call        print

    xor         eax, eax
    ret

;------------------------------------------------------------------------------
; print function
;   IN:
;       -ecx : address of string
;       -edx : string length
;
print:
    xor         eax, eax
    xor         ebx, ebx
    mov         al, 0x4             ; sys_write
    mov         bl, 0x1             ; stdout fd
    int         0x80
    ret


;------------------------------------------------------------------------------
; germ header
;

exe_str:        db "[x] germ executing",10
exe_str_len:    equ $-exe_str

comp_str:       db "[x] complete",10
comp_str_len:   equ $-comp_str

fail_str:       db "[x] failure",10
fail_str_len:   equ $-fail_str

germ_len:       equ end-_start
end:
