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

-------------

Progress:
=======

* Seems that there is a change to how nasm x64 handles labels:

        call label
    label:
        pop  r15
        sub  r15, label


    With x64 this produces obj file with:

        1:   e8 00 00 00 00          call   6 <label>

    0000000000000006 <label>:
        6:   41 5f                   pop    r15
        8:   49 81 ef 00 00 00 00    sub    r15,0x0


    X86 would have had 'sub <reg>,6'...  I haven't been able to google an
    explanation for this.
    
    This means the delta offset trick doesn't work with x64 assembly..  Wonder
    if I should just use x86 again.
