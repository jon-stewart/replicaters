replicaters
=======

The basic goal of this project is to have blocks of code that replicate within
an allocated portion of user process memory.

The advanced stages of this project is to have these code blocks acting like
organisms.  They compete for memory space and CPU time, a simulated survival of
the fittest with asexual and sexual replication (crossover and mutation of
genes).

--------------

To have code organisms replicating and competing in a memory space a chunk of
standalone code is required.  The organisms are not true organisms if they are
simply making use of the host functions.

I will use assembly, compile with nasm, strip the .text section with my disas.py program
and then read and write this 'shellcode' to the memory space.

--------------

When working on the idea of each germ having its own stack frame in the pool I
realised that if a germ faults, the whole program goes down.

Solution is of course to spawn a child process to execute our germ code.  If it
faults - clear the memory space and remove from the list.  I will use sys_clone
instead of fork as this will allow me to provide each organism with its own
stack.
