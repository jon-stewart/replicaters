repnmem
=======

Replicators in memory


asm_commands:

    nasm -f elf -o test.o test.asm
    ld -melf_i386 -o test test.o

    gcc -m32 -z execstack -o shell_test shell_test.c
