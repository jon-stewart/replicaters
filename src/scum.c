#include <repnmem.h>

LIST(scum);

void
froth(void)
{
    list_t     *ptr  = NULL;
    germ_t     *germ = NULL;

    for_each_list_ele(&scum, ptr) {
        germ = (germ_t *) ptr;
        assert(germ != NULL);

        printf("[*] spawning\n");
    }
}

void
release_scum(void)
{
    germ_t     *germ = NULL;

    while (!list_empty(&scum)) {
        germ = (germ_t *) list_rm_back(&scum);
        assert(germ != NULL);

        free(germ);
    }
}
