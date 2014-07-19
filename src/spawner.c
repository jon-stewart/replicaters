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
