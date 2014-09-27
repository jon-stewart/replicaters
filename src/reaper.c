#include <replicaters.h>
#include <pthread.h>
#include <signal.h>

static void reaper_reg_sig_handler(int);
static void reaper_signal(int);
static void reaper_kill(void);

LIST(damned);

void
reaper_init(void)
{
    reaper_reg_sig_handler(SIGSEGV);
    reaper_reg_sig_handler(SIGBUS);
}

static void
reaper_reg_sig_handler(int signo)
{
    if (signal(signo, reaper_signal) == SIG_ERR) {
        fprintf(stderr, "[!] can't set signal %d handler\n", signo);
        exit(1);
    }
}

static void
reaper_signal(int signo)
{
    pthread_t   tid;

    tid = pthread_self();

    printf("[!] Caught signal %d : 0x%lx\n", signo, tid);

    /*
     * TODO - Add and signal
     */
//    list_add(&damned, x);

    pthread_exit(0);
}

/*
 * TODO - cv_wait for germs to kill
 */
void
reaper_kill(void)
{
    pthread_t   tid = 0;
    list_t     *ptr = NULL;
    germ_t     *germ = NULL;

    list_t scum;

    for_each_list_ele(&scum, ptr) {
        germ = (germ_t *) ptr;
        assert(germ != NULL);

        // reap the germ
        if (germ->tid == tid) {
            printf("[!] Found the bad germ : %d\n", germ->generation);
        }
    }

}
