#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

volatile sig_atomic_t sig_flag = 0;

void handler(int sig) {
    sig_flag = 1;
}

int main() {
    signal(SIGWINCH, handler);

    while (1) {
        if (sig_flag) {
            sig_flag = 0;

            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

            printf("Terminal resized: %dx%d chars, %dx%d pixels\n", w.ws_col, w.ws_row, w.ws_xpixel, w.ws_ypixel);
        }
    }

    return 0;
}
