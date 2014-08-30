#ifndef _REPNMEM_H_
#define _REPNMEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <list.h>

#define VAT_SIZE            (1 << 9)
#define GERM_CODE           ("/tmp/output.sc")

typedef struct germ {
    list_t         ls;

    unsigned (*entry)(void *);

    unsigned int    magic;
    int             generation;
    pthread_t       tid;

} germ_t;

void vat_init(void);
void vat_destroy(void);

void scum_froth(void);
void scum_infect(void);
void scum_add(void *);
void scum_release(void);

void reaper_init(void);

#endif /* _REPNMEM_H_ */
