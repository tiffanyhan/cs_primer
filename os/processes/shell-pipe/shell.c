#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

int parse(char input[], char* output[], const char *delimiter) {
    char *token;
    char *rest = input;
    int count = 0;

    while ((token = strsep(&rest, delimiter)) != NULL) {
        if (*token == '\0') {
            continue; // skip empty tokens
        }
        output[count] = token;
        count++;
    }

    output[count] = NULL;
    return count;
}

void quit() {
    printf("[Process completed]");
    exit(0);
}

int builtin(char* argv[]) {
    if (strcmp(argv[0], "exit") == 0) {
        quit();
        // return 1;
    }

    return 0;
}

// only support pipes for programs
int spawn(char* argv[], int rfd, int wfd, int pipefds[][2], int pipefdsc) {
    pid_t pid = fork();

    if (pid == 0) {
        // child process    
        if (rfd != STDIN_FILENO) {
            dup2(rfd, STDIN_FILENO);
            close(rfd);
        }
        if (wfd != STDOUT_FILENO) {
            dup2(wfd, STDOUT_FILENO);
            close(wfd);
        }
        for (int i = 0; i < pipefdsc; i++) {
            close(pipefds[i][0]);
            close(pipefds[i][1]);
        }

        signal(SIGINT, SIG_DFL);
        execvp(argv[0], argv);
        perror("execvp error");
    }

    // parent process
    return pid;
}

// run should accept fd args for reading and writing
int run(char* argv[], int rfd, int wfd, int pipefds[][2], int pipefdsc) {
    if (builtin(argv) == 1) {
        return -1;
    };

    return spawn(argv, rfd, wfd, pipefds, pipefdsc);
}

int main() {
    signal(SIGINT, SIG_IGN);

    for (;;) {
        printf("\n🐚 ");

        char input[4096];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (feof(stdin)) {
                quit();
            } else {
                perror("fgets error");
                exit(1);
            }
        }

        char* cmdsv[100];
        int cmdsc = parse(input, cmdsv, "|");

        // create all the pipes needed ahead of time?
        int pipefds[cmdsc-1][2];

        // create n-1 pipes
        // if there's only 1 command, this won't run
        for (int i = 0; i < cmdsc - 1; i++) {
            pipe(pipefds[i]);
        }

        // first and last commands are special cases?
        // otherwise, the program needs to set its stdin from prev pipe
        // and set its stdout to next pipe
        //
        // i = 0: "ls", read should be stdin, write should be at pipefds[0]
        // i = 1: "wc", read should be from pipefds[0], write should be at pipefds[1]
        // i = 2; "echo", read should be from pipefds[1], write should be at pipefds[2]
        pid_t pids[cmdsc];

        for (int i = 0; i < cmdsc; i++) {
            char *cmd = cmdsv[i];
            char* argv[1024];
            int argc = parse(cmd, argv, " \t\n");

            if (argc) {
                int rfd, wfd;

                if (i > 0) { // if greater than first command
                    rfd = pipefds[i-1][0]; // read from prev pipe
                } else {
                    rfd = STDIN_FILENO; // first command should read from stdin
                }

                if (i < cmdsc - 1) { // if less than last command
                    wfd = pipefds[i][1]; // write to next pipe
                } else {
                    wfd = STDOUT_FILENO; // last command should write to stdout
                }

                pids[i] = run(argv, rfd, wfd, pipefds, cmdsc - 1);
            }
        }

        for (int i = 0; i < cmdsc - 1; i++) {
            close(pipefds[i][0]);
            close(pipefds[i][1]);
        }

        for (int i = 0; i < cmdsc; i++) {
            if (pids[-1] == -1) { continue; } 
            waitpid(pids[i], NULL, 0);
        }
    }

    return 0;
}
