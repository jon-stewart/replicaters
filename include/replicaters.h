#ifndef _REPNMEM_H_
#define _REPNMEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <list.h>
#include <pthread.h>

#define VAT_SIZE            (1 << 9)
#define GERM_CODE           ("/tmp/output.sc")

#define RW_INIT(rw) \
    pthread_rwlock_init((rw), NULL)

#define RW_WRLOCK(rw)   \
    pthread_rwlock_wrlock((rw))

#define RW_RDLOCK(rw)   \
    pthread_rwlock_rdlock((rw))

#define RW_UNLOCK(rw)   \
    pthread_rwlock_unlock((rw))

#define MTX_INIT(mtx)   \
    pthread_mutex_init((mtx), NULL)

#define MTX_ENTER(mtx)  \
    pthread_mutex_lock((mtx))

#define MTX_EXIT(mtx)   \
    pthread_mutex_unlock((mtx))

typedef struct germ {
    list_t         ls;

    unsigned (*entry)(void *);

    unsigned int    magic;
    int             generation;
    bool            dead;
    pthread_t       tid;
} germ_t;

/* vat.c */
void vat_init(void);
void vat_destroy(void);
void *vat_base_address(void);
void vat_scum_add(void *);
void vat_scum_release(void);
void stir(void);
void reap(pthread_t);

/* germ.c */
void foam(void);
void froth(void);
void infect(void);
void *spawn(void *);

/* reaper.c */
void reaper_init(void);

#endif /* _REPNMEM_H_ */
