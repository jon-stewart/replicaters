#!/bin/sh

make clean

make

nasm -f elf -o test.o -I asm/ asm/test.asm

python ../disas/disas.py -s test.o
