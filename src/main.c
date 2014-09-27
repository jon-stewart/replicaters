#include <replicaters.h>

int
main(int argc, char **argv)
{
    reaper_init();

    vat_init();

    infect();

    foam();
    froth();

    vat_destroy();

    return (0);
}
