#ifndef _SPAWNER_H_
#define _SPAWNER_H_

#include <dlist.h>

#define SPAWN_POOL_SIZE     (1 << 9)

typedef struct cell_ref {
    dlist_t         link;

    void           *address;
    unsigned int    magic;
    int             generation;

} cell_ref_t;

#endif /* _SPAWNER_H_ */
