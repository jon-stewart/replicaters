#include <replicaters.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

static void reaper_reg_sig_handler(int);
static void reaper_signal(int);

void
reaper_init(void)
{
    reaper_reg_sig_handler(SIGSEGV);
    reaper_reg_sig_handler(SIGBUS);
}

/*
 * Function:
 *      reaper_reg_sig_handler
 *
 * Description:
 *      Register handler to catch specified signals.
 */
static void
reaper_reg_sig_handler(int signo)
{
    if (signal(signo, reaper_signal) == SIG_ERR) {
        fprintf(stderr, "[!] can't set signal %d handler\n", signo);
        exit(1);
    }
}

/*
 * Function:
 *      reaper_signal
 *
 * Description:
 *      Catch and handle the signal by marking the germ as dead.  Able to
 *      identify germ by thread id.
 */
static void
reaper_signal(int signo)
{
    pthread_t   tid;

    tid = pthread_self();

    debug("[!] Caught signal %d : 0x%lx\n", signo, tid);

    mark_dead(tid);

    pthread_exit(0);
}

/*
 * Function:
 *      reaper_cleanse
 *
 * Description:
 *      Remove all trace of the germ from the vat.  Remove from list and zero
 *      memory.
 */
void
reaper_cleanse(germ_t *germ)
{
    assert(germ        != NULL);
    assert(germ->entry != NULL);

    memset(germ->entry, 0, germ->len);

    list_rm(&germ->ls);
    free(germ);
}
