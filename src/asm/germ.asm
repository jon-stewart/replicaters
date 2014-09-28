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
[section .text]

[BITS 64]

%include "germ.mac"

global _start

_start:
    nop
    
    ; prolog - stack frame creation
    push        rbp
    mov         rbp, rsp
    sub         rsp, 0x10
    ; stack frame:
    ;   - 0x8  start address
    ;   - 0x10 cb address

    call        delta
delta:
    pop         r15
    sub         r15, delta          ; our start address
    mov         [rbp-0x8], r15      ; store start address
    mov         [rbp-0x10], rdi     ; store cb address

    ; print out the begin execution message
    _print      0x8, exe_str, exe_str_len

    call        search

;    call        copy

    ; print completion message
;    _print      0x8, comp_str, comp_str_len

    ; make call to the callback
    mov         rax, [rbp-0x10]     ; cb address
    mov         rsi, reach          ; arg1: length
    call        rax

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
;       rcx-address of string
;       rdx-string length
; returns:
;       rax-#bytes written
;
print:
    xor         rax, rax
    mov         al,  0x1            ; sys_write
    mov         rdi, 0x1            ; stdout fd
    syscall
    ret

;------------------------------------------------------------------------------
; search:
;       now begin to search for enough free space
; in:
; returns:
;       rdi-address found
;
search:
.find_null:
    xor         rax, rax
    mov         rdi, [rbp-0x8]      ; start address
    add         rdi, germ_len       ; move pointer to beyond end
    xor         rcx, rcx
    add         rcx, reach          ; area in which we can replicate
    repne       scasw               ; repeat scasw as long as [rdi] does not equal rax

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
stack_sz:       equ 0ffh
reach:          equ 080h

germ_len:       equ end-_start
end:
