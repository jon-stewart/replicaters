#include <replicaters.h>
#include <pthread.h>
#include <signal.h>

static void reaper_reg_sig_handler(int);
static void reaper_signal(int);

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

    mark_dead(tid);

    pthread_exit(0);
}
