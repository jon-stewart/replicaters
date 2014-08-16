#include <replicaters.h>
#include <string.h>

void *spawn_pool = NULL;

void
init_spawn_pool(void)
{
    spawn_pool = malloc(SPAWN_POOL_SIZE);
    assert(spawn_pool != NULL);
}

void
destroy_spawn_pool(void)
{
    assert(spawn_pool != NULL);
    free(spawn_pool);
}
