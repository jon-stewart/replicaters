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

I will use assembly, compile with nasm, link with ld (-Ttext=0) strip out the
.text section with my binextract.py program and then read and write this
'shellcode' to the vat memory space.
