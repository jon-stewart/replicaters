#!/bin/sh

nasm_debug=""
c_debug=""
if [ -n $1 ]
then
    if [ "$1" == "debug" ]
    then
        nasm_debug="-DDEBUG"
        c_debug="DEBUG=1"
    fi
fi

make clean

make $c_debug

nasm -f elf64 -o /tmp/germ.o -I src/asm/ src/asm/germ.asm $nasm_debug

ld -Ttext 0 -o /tmp/germ /tmp/germ.o

python ../binextract/binextract.py -f /tmp/germ -s germ.sc
