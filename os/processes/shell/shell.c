#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

int tokenize(char input[], char* argv[]) {
    char *token;
    char *rest = input;
    const char *delimiter = " \t\n";
    int argc = 0;

    while ((token = strsep(&rest, delimiter)) != NULL) {
        if (*token == '\0') {
            continue; // skip empty tokens
        }
        argv[argc] = token;
        argc++;
    }

    argv[argc] = NULL;
    return argc;
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

int spawn(char* argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        // child process
        signal(SIGINT, SIG_DFL);
        execvp(argv[0], argv);
        perror("execvp error");
        return 1;
    } else {
        // parent process
        wait(NULL);
    }

    return 0;
}

void run(char* argv[]) {
    if (builtin(argv) == 1) {
        return;
    };

    spawn(argv);
}

int main() {
    signal(SIGINT, SIG_IGN);

    for (;;) {
        printf("\n🐚 ");

        char input[100];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (feof(stdin)) {
                quit();
            } else {
                perror("fgets error");
                exit(1);
            }
        }

        char* argv[100];
        int argc = tokenize(input, argv);

        if (argc) {
            run(argv);
        }
    }

    return 0;
}
