#include <replicaters.h>
#include <string.h>
#include <pthread.h>

LIST(scum);

extern void *spawn_pool;

void
callback(void *addr, unsigned length)
{
    printf("[*] callback : %p, %x\n", addr, length);
}

void *
child_(void *arg)
{
    int         ret;
    germ_t     *germ = (germ_t *) arg;

    printf("[*] START\n");
    ret = germ->entry((void *) callback);
    if (ret == 0) {
        printf("[*] PASS\n");
    }

    return (NULL);
}

void
froth(void)
{
    list_t     *ptr  = NULL;
    germ_t     *germ = NULL;
    pthread_t   thd;
    int         ret;


    for_each_list_ele(&scum, ptr) {
        germ = (germ_t *) ptr;
        assert(germ != NULL);

        printf("[*] spawning\n");

        ret = pthread_create(&thd, NULL, child_, (void *) germ);
        if (ret) {
            fprintf(stderr, "errors : %d\n", ret);
        }
    }

    pthread_join(thd, NULL);

    printf("[*] Froth finish\n");

}

void
infect(void)
{
    FILE       *file    = NULL;
    char       *buffer  = NULL;
    void       *addr    = NULL;
    unsigned    len     = 0;


    file = fopen(GERM_CODE, "rb");

    fseek(file, 0L, SEEK_END);

    len = ftell(file);

    fseek(file, 0L, SEEK_SET);

    buffer = malloc(sizeof(char) * len);

    fread(buffer, len, len, file);

    fclose(file);

    addr = spawn_pool;

    memcpy(addr, buffer, len);

    free(buffer);

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
