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

/* vat.c */
void vat_init(void);
void vat_destroy(void);
void *vat_base_address(void);
void vat_scum_add(void *);
void vat_scum_release(void);

/* germ.c */
void foam(void);
void froth(void);
void infect(void);
void *spawn(void *);

/* reaper.c */
void reaper_init(void);

#endif /* _REPNMEM_H_ */
