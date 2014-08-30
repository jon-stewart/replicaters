#include <replicaters.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

extern list_t scum;

void
sig_usr(int signo)
{
    pthread_t   tid = pthread_self();
    list_t     *ptr = NULL;
    germ_t     *germ = NULL;

    printf("[!] Caught signal %d\n", signo);

    for_each_list_ele(&scum, ptr) {
        germ = (germ_t *) ptr;
        assert(germ != NULL);

        // reap the germ
        if (germ->tid == tid) {
            printf("[!] Found the bad germ : %d\n", germ->generation);
            germ->dead = true;
        }
    }

    pthread_exit(0);
}

void register_sig_handler(int signo)
{
    if (signal(signo, sig_usr) == SIG_ERR) {
        fprintf(stderr, "[!] can't set %d handler\n", signo);
        exit(1);
    }
}

int
main(int argc, char **argv)
{
    /* Register signal handler */
    register_sig_handler(SIGSEGV);
    register_sig_handler(SIGBUS);

    init_spawn_pool();

    infect();

    int i;
    for (i = 0; i < 1; ++i) {
        froth();
    }

    release_scum();

    destroy_spawn_pool();

    return (0);
}
