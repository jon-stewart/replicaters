#!/bin/sh

make clean

make

nasm -f elf64 -o /tmp/germ.o -I src/asm/ src/asm/germ.asm

ld -Ttext 0 -o /tmp/germ /tmp/germ.o

python ../disas/disas.py -s /tmp/germ
