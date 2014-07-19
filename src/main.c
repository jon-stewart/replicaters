#include <repnmem.h>

int
main(int argc, char **argv)
{
    init_spawn_pool();

    infect();

    froth();

    destroy_spawn_pool();

    return (0);
}
