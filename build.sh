#!/bin/sh

make clean

make

nasm -f elf -o /tmp/test.o -I asm/ asm/test.asm

python ../disas/disas.py -s /tmp/test.o
