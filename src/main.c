#include <replicaters.h>
#include <signal.h>

void
sig_usr(int sig_num)
{
    printf("[!] Caught signal %d\n", sig_num);
    exit(1);
}

void register_sig_handler(int sig_num)
{
    if (signal(SIGSEGV, sig_usr) == SIG_ERR) {
        fprintf(stderr, "[!] can't set %d handler\n", sig_num);
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
