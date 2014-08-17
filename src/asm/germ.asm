;------------------------------------------------------------------------------
; Name:
;   germ x64
;
; in:
;   edi-address to callback function
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
;   This must be encoded in x86 assembly as nasm x64 will not resolve labels
;   if used as memory addresses.
;
;
[section .text]

[BITS 64]
default rel

%include "germ.mac"

global _start

_start:
    nop
    
    ; prolog - stack frame creation
    push        rbp
    mov         rbp, rsp
    sub         rsp, 0x8
    ; stack frame:
    ;   - 0x8 start address

    call        delta
delta:
    pop         r15
    sub         r15, delta          ; our start address
    mov         [rbp-0x8], r15      ; store start address

    ; print out the begin execution message
    _print      0x8, exe_str, exe_str_len

;    call        search

;    call        copy

    ; print completion message
    _print      0x8, comp_str, comp_str_len

exit:
    ; epilog - stack frame cleanup
    mov         rsp, rbp
    pop         rbp
    xor         rax, rax
    ret

;------------------------------------------------------------------------------
; print:
;       use the linux write syscall to output text string to stdout
; in:
;       ecx-address of string
;       edx-string length
; returns:
;       eax-#bytes written
;
print:
    xor         rax, rax
    mov         al,  0x1            ; sys_write
    mov         rdi, 0x1            ; stdout fd
    syscall
    ret

;------------------------------------------------------------------------------
; search:
;
; in:
; returns:
;
search:
    ; now begin to search for enough free space
    xor         rax, rax            ; search for NULL
    mov         rdi, rbp            ; start address
    add         rdi, germ_len       ; start address + length
    xor         rcx, rcx
    add         rcx, germ_len       ; counter

.loop:
    add         rdi, 0x1
    cmp         [rdi], al
    jne         .nospace

    dec         cl
    cmp         rcx, 0x0
    jne         .loop

.nospace:
    xor         rax, rax
    mov         rax, 0x1
    jmp         .exit
.exit:
    ret

;------------------------------------------------------------------------------
; copy:
;
; in:
; returns:
;
copy:
    mov         rdi, rbp
    add         rdi, germ_len
    mov         rdi, germ_len
    rep         movsb
    ret

;------------------------------------------------------------------------------
; fail:
;
; in:
; returns:
;
failure:
    ; print out failure message
    _print      0x8, fail_str, fail_str_len

    mov         rbx, 0xDEADBEEF
    mov         rax, 0x1

    pop         r15                 ; get rid of previous return address
    jmp         exit


;------------------------------------------------------------------------------
; debug string
;
exe_str:        db "[x] germ executing",10,0
exe_str_len:    equ $-exe_str

comp_str:       db "[x] complete",10,0
comp_str_len:   equ $-comp_str

fail_str:       db "[x] failure",10,0
fail_str_len:   equ $-fail_str

;------------------------------------------------------------------------------
; germ info
;
stack_sz:       dw 0ffh

germ_len:       equ end-_start
end:
