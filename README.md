repnmem
=======

Replicators in memory


To have code organisms replicating and competing in a memory space a chunk of
standalone code is required.  The organisms are not true organisms if they are
simply making use of the host functions.

I will use assembly, compile with nasm, strip the .text section with my disas.py program
and then read and write this 'shellcode' to the memory space.

--------------

Basic proof of concept is working.

Each time the code is executed in the pool a new copy is made and added to the
list.
