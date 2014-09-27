#include <replicaters.h>

int
main(int argc, char **argv)
{
    reaper_init();

    vat_init();

    infect();

    stir();
    froth();

    stir();
    froth();

    vat_destroy();

    return (0);
}
