replicaters
=======

Replicaters in memory



--------------

To have code organisms replicating and competing in a memory space a chunk of
standalone code is required.  The organisms are not true organisms if they are
simply making use of the host functions.

I will use assembly, compile with nasm, strip the .text section with my disas.py program
and then read and write this 'shellcode' to the memory space.

--------------


When working on the idea of each germ having its own stack frame in the pool I
realised that if germs fault, the whole program goes down.

Solution is of course to spawn a child process to execute our germ code.  If it
faults - clear the memory space and remove from the list.
