#include <replicaters.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

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

    memset(vat.pool, 0, VAT_SIZE);

    // printf("vat - s:%p, e:%p\n", vat.pool, vat.pool + VAT_SIZE);

    list_init(&vat.scum);
    RW_INIT(&vat.scum_rw);

    list_init(&vat.fresh);
    MTX_INIT(&vat.fresh_mtx);
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
vat_scum_add(void *addr, size_t len, unsigned gen)
{
    germ_t     *germ = NULL;


    germ = malloc(sizeof (*germ));
    assert(germ != NULL);

    germ->magic      = GERM_MAGIC;
    germ->entry      = addr;
    germ->len        = len;
    germ->generation = gen;
    germ->dead       = false;
    germ->tid        = 0;

    MTX_ENTER(&vat.fresh_mtx);

    list_add(&vat.fresh, &germ->ls);

    MTX_EXIT(&vat.fresh_mtx);

    printf("[%s] Added germ: %p, gen: %u\n", __FUNCTION__,
            germ->entry, germ->generation);
}

void
vat_scum_release(void)
{
    germ_t     *germ = NULL;
    list_t     *ptr  = NULL;

    RW_WRLOCK(&vat.scum_rw);

    while (!list_empty(&vat.scum)) {
        ptr = list_rm_back(&vat.scum);
        assert(ptr != NULL);
        germ = LIST_ENTRY(ptr, germ_t, ls);
        assert(germ->magic == GERM_MAGIC);

        free(germ);
    }

    RW_UNLOCK(&vat.scum_rw);
}

static void
mygetch(void)
{
  struct termios old, new;

  tcgetattr(STDIN_FILENO, &old);
  new = old;
  new.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new);
  getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &old);
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

    printf("---------------------------------------------------------------\n");
    printf("[*] Froth begins\n");

    RW_RDLOCK(&vat.scum_rw);

    LIST_FOR_EACH(&vat.scum, ptr) {
        germ = LIST_ENTRY(ptr, germ_t, ls);
        assert(germ->magic == GERM_MAGIC);

        // printf("[*] Spawning : %p\n", germ->entry);

        ret = pthread_create(&thd, NULL, spawn, (void *) germ);
        if (ret) {
            fprintf(stderr, "errors : %d\n", ret);
        } else {
            pthread_join(thd, NULL);
        }
    }

    RW_UNLOCK(&vat.scum_rw);

    printf("[*] Froth finish\n");

    printf("[*] Enter to continue to next generation\n>");
    mygetch();
}

/*
 * Function:
 *      stir
 *
 * Description:
 *      Post processing of spawn step.  Reap any dead germ and add any
 *      newly replicated germ to the scum list.
 */
void
stir(void)
{
    germ_t *germ = NULL;
    list_t *ptr  = NULL;
    list_t *nxt  = NULL;


    RW_WRLOCK(&vat.scum_rw);

    /* Reap the dead */
    LIST_FOR_EACH_SAFE(&vat.scum, nxt, ptr) {
        germ = LIST_ENTRY(ptr, germ_t, ls);
        assert(germ->magic == GERM_MAGIC);

        if (germ->dead == true) {
            reaper_cleanse(germ);
        }
    }

    MTX_ENTER(&vat.fresh_mtx);

    while (!list_empty(&vat.fresh)) {
        ptr = list_rm_front(&vat.fresh);
        assert(ptr != NULL);
        germ = LIST_ENTRY(ptr, germ_t, ls);
        assert(germ->magic == GERM_MAGIC);

        list_add(&vat.scum, &germ->ls); 
    }

    MTX_EXIT(&vat.fresh_mtx);

    RW_UNLOCK(&vat.scum_rw);
}

/*
 * Function:
 *      mark_dead
 *
 * Description:
 *      Find germ to mark as dead, this will be reaped in the next stir call.
 */
void
mark_dead(pthread_t tid)
{
    list_t *ptr  = NULL;
    germ_t *germ = NULL;

    RW_RDLOCK(&vat.scum_rw);

    LIST_FOR_EACH(&vat.scum, ptr) {
        germ = LIST_ENTRY(ptr, germ_t, ls);
        assert(germ->magic == GERM_MAGIC);

        if (germ->tid == tid) {
            germ->dead = true;
        }
    }

    RW_UNLOCK(&vat.scum_rw);
}
