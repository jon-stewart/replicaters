#include <replicaters.h>
#include <string.h>
#include <pthread.h>

/*
 * XXX This could be neater
 */
typedef struct list_head {
    list_t list;
    pthread_mutex_t mtx;
} list_head_t;

/*
 * XXX Do this better
 */
typedef struct damn {
    list_t      list;
    pthread_t   tid;
} damn_t;

typedef struct vat {
    void *pool;

    list_head_t scum;
    list_head_t germinate;
    list_head_t damned;

} vat_t;

static vat_t vat;

void
vat_init(void)
{
    vat.pool = malloc(VAT_SIZE);
    assert(vat.pool != NULL);

    list_init(&vat.scum.list);
    pthread_mutex_init(&vat.scum.mtx, NULL);
    /* XXX wrap pthreads in macro */

    list_init(&vat.germinate.list);
    pthread_mutex_init(&vat.germinate.mtx, NULL);

    list_init(&vat.damned.list);
    pthread_mutex_init(&vat.damned.mtx, NULL);
}

void
vat_destroy(void)
{
    vat_scum_release();

    assert(vat.pool != NULL);
    free(vat.pool);
}

void *
vat_base_address(void)
{
    assert(vat.pool != NULL);

    return (vat.pool);
}

void
vat_scum_add(void *addr)
{
    germ_t     *germ = NULL;


    germ = malloc(sizeof (*germ));
    assert(germ != NULL);

    germ->entry      = addr;
    germ->magic      = 0;
    germ->generation = 1;

    pthread_mutex_lock(&vat.germinate.mtx);

    list_add(&vat.germinate.list, &germ->ls);

    pthread_mutex_unlock(&vat.germinate.mtx);
}

void
vat_scum_release(void)
{
    germ_t     *germ = NULL;

    pthread_mutex_lock(&vat.scum.mtx);

    while (!list_empty(&vat.scum.list)) {
        germ = (germ_t *) list_rm_back(&vat.scum.list);
        assert(germ != NULL);

        free(germ);
    }

    pthread_mutex_unlock(&vat.scum.mtx);
}

/*
 * Froth up the scum and get the germs spawning
 */
void
froth(void)
{
    list_t     *ptr  = NULL;
    germ_t     *germ = NULL;
    pthread_t   thd;
    int         ret;

    pthread_mutex_lock(&vat.scum.mtx);

    for_each_list_ele(&vat.scum.list, ptr) {
        germ = (germ_t *) ptr;
        assert(germ != NULL);

        printf("[*] spawning\n");

        ret = pthread_create(&thd, NULL, spawn, (void *) germ);
        if (ret) {
            fprintf(stderr, "errors : %d\n", ret);
        } else {
            pthread_join(thd, NULL);
        }
    }

    pthread_mutex_unlock(&vat.scum.mtx);

    printf("[*] Froth finish\n");
}

/*
 * Add germinated germs to the scum
 */
void
foam(void)
{
    list_t  transition;
    germ_t *germ;

    list_init(&transition);

    pthread_mutex_lock(&vat.germinate.mtx);

    while (!list_empty(&vat.germinate.list)) {
        germ = (germ_t *) list_rm_front(&vat.germinate.list);
        assert(germ != NULL);

        list_add(&transition, &germ->ls);
    }

    pthread_mutex_unlock(&vat.germinate.mtx);


    pthread_mutex_lock(&vat.scum.mtx);

    while (!list_empty(&transition)) {
        germ = (germ_t *) list_rm_front(&transition);
        assert(germ != NULL);

        list_add(&vat.scum.list, &germ->ls);
    }

    pthread_mutex_unlock(&vat.scum.mtx);
}

void
damn(pthread_t tid)
{
    damn_t  *damn = malloc(sizeof (*damn));

    damn.tid = tid;

    pthread_mutex_lock(&vat.damned.mtx);    

    list_add(&vat.damned.list, &damn.ls);

    pthread_mutex_unlock(&vat.damned.mtx);    
}

void
reap(void)
{
    list_t *ptr  = NULL;
    damn_t *damn = NULL;


    pthread_mutex_lock(&vat.damned.mtx);

    for_each_list_ele(&vat.damned.list, ptr) {
        damn = (damn_t *) ptr;

        
    }

    pthread_mutex_unlock(&vat.damned.mtx);
}
