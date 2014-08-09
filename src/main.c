#include <replicaters.h>

int
main(int argc, char **argv)
{
    init_spawn_pool();

    infect();

    int i;
    for (i = 0; i < 1; ++i) {
        froth();
    }

    release_scum();

    destroy_spawn_pool();

    return (0);
}
