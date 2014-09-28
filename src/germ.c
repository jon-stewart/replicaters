#include <replicaters.h>
#include <string.h>

static void
callback(void *addr, unsigned length)
{
    printf("[*] callback : %p, 0x%x\n", addr, length);

    assert(addr != NULL);
    assert(length > 0);
}

void *
spawn(void *arg)
{
    germ_t  *germ = (germ_t *) arg;

    /* Dead germs should have been reaped */
    assert(germ->dead == false);
    assert(germ->tid  == 0);

    germ->tid = pthread_self();

    printf("[*] START 0x%lx\n", germ->tid);

    if (germ->entry((void *) callback) == 0) {
        germ->tid = 0;

        printf("[*] PASS\n");
    }

    return (NULL);
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

    addr = vat_base_address();

    memcpy(addr, buffer, len);

    free(buffer);

    vat_scum_add(addr);
}
