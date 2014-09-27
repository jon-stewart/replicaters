#include <replicaters.h>
#include <string.h>
#include <pthread.h>

static void
callback(void *addr, unsigned length)
{
    printf("[*] callback : %p, %x\n", addr, length);
}

void *
spawn(void *arg)
{
    int         ret;
    germ_t     *germ = (germ_t *) arg;
    pthread_t   tid  = pthread_self();

    if (germ->tid) {
        return (NULL);
    }

    germ->tid = tid;

    printf("[*] START : 0x%lx\n", tid);
    ret = germ->entry((void *) callback);
    if (ret == 0) {
        printf("[*] PASS\n");
        germ->tid = 0;
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
