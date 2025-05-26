#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

volatile uint64_t handled = 0;
volatile int signo = 0;

int counter = 0;

void handle(int sig) {
  signo = sig;
  handled |= (1ULL << sig);
}

// run as `./a.out > out.txt`
int main(int argc, char* argv[]) {
    // Register all valid signals
    for (int i = 0; i < NSIG; i++) {
        signal(i, handle);
    }

    alarm(3); // SIGALRM

    struct rlimit rlcpu;
    rlcpu.rlim_cur = 3; // 3 sec
    rlcpu.rlim_max = 3; // 3 sec
    setrlimit(RLIMIT_CPU, &rlcpu); // SIGXCPU

    struct rlimit rlfsize;
    rlfsize.rlim_cur = 5; // 5 bytes
    rlfsize.rlim_max = 5; // 5 bytes
    setrlimit(RLIMIT_FSIZE, &rlfsize); // SIGXFSZ

    FILE *fptr;
    char *fname = "example.txt";
    fptr = fopen(fname, "w");

    if (0 == fork()) {
      exit(0); // SIGCHLD
    }

    // spin
    for (;;) {
      if (signo) {
        printf("Caught %d: %s (%d total)\n", signo, sys_siglist[signo],
          __builtin_popcount(handled));
        fflush(stdout); // important when redirecting output to a file for SIGHUP

        if (signo == SIGINT) {
          exit(0);
        }

        if (signo == SIGTSTP) {
          signal(SIGTSTP, SIG_DFL);
          raise(SIGTSTP);
          printf("Resumed from suspension\n");
        }

        signo = 0;
      }
      
      counter++; // SIGXCPU

      if (counter == 1) {
        fprintf(fptr, "hello, world!\n"); // SIGXFSZ
        fflush(fptr);
      }
    }
}
