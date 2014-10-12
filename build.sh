#!/bin/sh

make clean

make

debug=""
if [ -n $1 ]
then
    if [ "$1" == "debug" ]
    then
        debug="-DDEBUG"
    fi
fi

nasm -f elf64 -o /tmp/germ.o -I src/asm/ src/asm/germ.asm $debug

ld -Ttext 0 -o /tmp/germ /tmp/germ.o

python ../binextract/binextract.py -s /tmp/germ
