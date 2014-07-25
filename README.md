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

--------------

When working on the idea of each germ having its own stack frame in the pool I
realised that if germs fault, the whole program goes down.

If I am to have crossover and mutation in replication then this is likely to
occur... shit.

I may have ran down the wrong path on this.  Perhaps it would be simpler to
have had these germs running rogue in the memory space of a virtual machine..
Leave the spawning and cleanup to the OS...
