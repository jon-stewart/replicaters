#include <replicaters.h>

int
main(int argc, char **argv)
{
    int i;

    reaper_init();

    vat_init();

    infect();

    for (i = 0; i < 1000; ++i) {
        stir();
        froth();
    }

    vat_destroy();

    return (0);
}
