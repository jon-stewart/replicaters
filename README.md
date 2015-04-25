replicaters
=======

Have (good) replicating germs in shared memory.

--------------

The replicating germ assembly is compiled with nasm and linked with ld (-Ttext=0). The
.text section is then stripped out with my binextract.py program and this 'shellcode'
is then written to the vat memory space.
