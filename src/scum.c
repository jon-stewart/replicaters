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

        germ->entry();
    }
}

void
infect(void)
{
    FILE       *file = NULL;
    char       *buf  = NULL;
    void       *addr = NULL;
    unsigned    len  = 0;


    file = fopen("/tmp/output.sc", "rb");

    fseek(file, 0L, SEEK_END);

    len = ftell(file);

    fseek(file, 0L, SEEK_SET);

    *buf = malloc(sizeof(char) * len);

    fread(*buf, len, len, file);

    fclose(file);

    addr = spawn_pool;

    memcpy(addr, buf, len);

    free(buf);

    add_scum(addr);
}

void
add_scum(void *addr)
{
    germ_t     *germ = NULL;


    germ = malloc(sizeof (*germ));
    assert(germ != NULL);

    germ->entry      = addr;
    germ->magic      = 0;
    germ->generation = 0;

    list_add(&scum, &germ->ls);
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
