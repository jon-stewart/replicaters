#include <replicaters.h>

int
main(int argc, char **argv)
{
    reaper_init();

    vat_init();

    infect();

    froth();

    vat_destroy();

    return (0);
}
