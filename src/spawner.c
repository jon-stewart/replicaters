#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <spawner.h>
#include <string.h>

void *spawn_pool = NULL;

list_t cell_ref_list;

/*
 *
 */ 
static void
init_pool(void)
{
    printf("[*] %s\n", __FUNCTION__);

    spawn_pool = malloc(SPAWN_POOL_SIZE);
    assert(spawn_pool != NULL);

    list_init(&cell_ref_list);
}

/*
 *
 */
static void
destroy_pool(void)
{
    cell_ref_t      *cref = NULL;

    printf("[*] %s\n", __FUNCTION__);

    free(spawn_pool);

    while (!list_empty(&cell_ref_list)) {
        cref = (cell_ref_t *) list_rm_back(&cell_ref_list);
        assert(cref != NULL);

        free(cref);
    }
}

static unsigned
get_shellcode(char **buf)
{
    FILE       *file = fopen("/tmp/output.sc", "rb");
    unsigned    len;


    fseek(file, 0L, SEEK_END);
    len = ftell(file);

    fclose(file);

    file = fopen("/tmp/output.sc", "rb");

    *buf = malloc(sizeof(char) * len);

    fread(*buf, len, len, file);

    fclose(file);

    return len;
}

static void
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

static void
spawn_loop(void)
{
    list_t         *ptr;
    cell_ref_t      *cref = NULL;

    printf("[*] %s\n", __FUNCTION__);

    for_each_list_ele(&cell_ref_list, ptr) {
        cref = (cell_ref_t *) ptr;
        assert(cref != NULL);

        printf("[*] spawning cell : \n");
    }
}

int main(int argc, char **argv)
{
    printf("    [REPNMEM]\n\n");

    init_pool();

    infect();

    spawn_loop();

    destroy_pool();

    return (0);
}
