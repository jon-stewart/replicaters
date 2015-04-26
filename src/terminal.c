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
