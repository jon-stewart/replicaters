#include <replicaters.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void
terminal_key_input(void)
{
    struct termios old, new;

    printf(">");

    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    printf("\n");
}

/*
 *
 * fflush(stdout);
 * printf("\b%c", char);
 */
void
terminal_print_scum(list_t *scum)
{
    list_t             *ptr       = NULL;
    germ_t             *germ      = NULL;
    struct winsize      w;
    unsigned long long  base_addr = 0xFFFFFFFFFFFFFFFF;
    size_t              min       = 0xFFFFFF;
    unsigned            column    = 0;
    unsigned            row       = 0;
    unsigned            blocks    = 0;
    int                 i, j;

    /* Get terminal size */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    /* Round down the number of column */
    column = ((w.ws_col % 2) ? w.ws_col - 1 : w.ws_col);
    column -= 4;

    base_addr = (unsigned long long) vat_base_address();

    /* Find smallest germ length */
    LIST_FOR_EACH(scum, ptr) {
        germ = LIST_ENTRY(ptr, germ_t, ls);
        assert(germ->magic == GERM_MAGIC);

        if (germ->len < min) {
            min = germ->len;
        }
    }

    blocks = VAT_SIZE / min;
    row = blocks / column;

    printf("smallest block size: %u (bytes) | #blocks in table: %u | row: %u\n",
            (unsigned) min, blocks, row);

    germ = LIST_ENTRY(scum->next, germ_t, ls);
    assert(germ->magic == GERM_MAGIC);

    /* Print the table of scum */
    printf("  ");
    for (i = 0; i < column; ++i) {
        printf("-");
    }
    printf("\n");
    for (i = 0; i < row; ++i) {
        printf(" |");
        for (j = 0; j < column; ++j) {

            if ((germ != NULL) && ((((unsigned long long) germ->entry) - base_addr) / min) == j) {
                printf("%d", germ->generation);

                if (germ->ls.next != scum) {
                    germ = LIST_ENTRY(germ->ls.next, germ_t, ls);
                    assert(germ->magic == GERM_MAGIC);
                } else {
                    germ = NULL;
                }
            }

            printf(" ");
        }
        printf("|\n");
    }
    printf("  ");
    for (i = 0; i < column; ++i) {
        printf("-");
    }
    printf("\n");
}
