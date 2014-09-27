#include <replicaters.h>
#include <string.h>

typedef struct vat {
    void *pool;

    list_t scum;
    pthread_rwlock_t scum_rw;

    list_t fresh;
    pthread_mutex_t fresh_mtx;
} vat_t;

static vat_t vat;

void
vat_init(void)
{
    vat.pool = malloc(VAT_SIZE);
    assert(vat.pool != NULL);

    list_init(&vat.scum);
    pthread_rwlock_init(&vat.scum_rw, NULL);

    list_init(&vat.fresh);
    pthread_mutex_init(&vat.fresh_mtx, NULL);
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
    germ->dead       = false;
    germ->tid        = 0;

    pthread_mutex_lock(&vat.fresh_mtx);

    list_add(&vat.fresh, &germ->ls);

    pthread_mutex_unlock(&vat.fresh_mtx);
}

void
vat_scum_release(void)
{
    germ_t     *germ = NULL;

    pthread_rwlock_wrlock(&vat.scum_rw);

    while (!list_empty(&vat.scum)) {
        germ = (germ_t *) list_rm_back(&vat.scum);
        assert(germ != NULL);

        free(germ);
    }

    pthread_rwlock_unlock(&vat.scum_rw);
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


    pthread_rwlock_rdlock(&vat.scum_rw);

    for_each_list_ele(&vat.scum, ptr) {
        germ = (germ_t *) ptr;

        printf("[*] spawning\n", germ);

        ret = pthread_create(&thd, NULL, spawn, (void *) germ);
        if (ret) {
            fprintf(stderr, "errors : %d\n", ret);
        } else {
            pthread_join(thd, NULL);
        }
    }

    pthread_rwlock_unlock(&vat.scum_rw);

    printf("[*] Froth finish\n");
}

/*
 * Add fresh germs to the scum
 */
void
stir(void)
{
    germ_t *germ = NULL;
    list_t *ptr  = NULL;
    list_t *nxt  = NULL;


    pthread_rwlock_wrlock(&vat.scum_rw);

    /* Reap the dead */
    for_each_list_ele_safe(&vat.scum, nxt, ptr) {
        germ = (germ_t *) ptr;

        if (germ->dead == true) {
            list_rm(&germ->ls);
            free(germ);
        }
    }

    pthread_mutex_lock(&vat.fresh_mtx);

    while (!list_empty(&vat.fresh)) {
        germ = (germ_t *) list_rm_front(&vat.fresh);

        list_add(&vat.scum, &germ->ls); 
    }

    pthread_mutex_unlock(&vat.fresh_mtx);

    pthread_rwlock_unlock(&vat.scum_rw);
}

void
reap(pthread_t tid)
{
    list_t *ptr  = NULL;
    germ_t *germ = NULL;

    pthread_rwlock_rdlock(&vat.scum_rw);

    for_each_list_ele(&vat.scum, ptr) {
        germ = (germ_t *) ptr;

        if (germ->tid == tid) {
            germ->dead = true;
        }
    }

    pthread_rwlock_unlock(&vat.scum_rw);
}
