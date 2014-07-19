#ifndef _SPAWNER_H_
#define _SPAWNER_H_

#include <list.h>

#define SPAWN_POOL_SIZE     (1 << 9)

typedef struct cell_ref {
    list_t         ls;

    void           *address;
    unsigned int    magic;
    int             generation;

} cell_ref_t;

void cell_registration(void);

void infect(void);

#endif /* _SPAWNER_H_ */
