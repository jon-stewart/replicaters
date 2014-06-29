#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <spawner.h>

void *spawn_pool = NULL;

/*
 *
 */ 
static void
init_pool(void)
{
    printf("[*] initialization\n");

    spawn_pool = malloc(SPAWN_POOL_SIZE);
    assert(spawn_pool != NULL);
}

/*
 *
 */
static void
destroy_pool(void)
{
    printf("[*] destroying pool\n");

    free(spawn_pool);
}

int main(int argc, char **argv)
{
    printf("[REPNMEM]\n");

    init_pool();

    destroy_pool();

    return (0);
}
