#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

// Proof of concept: do `ls | wc`
int main () {
    int pipefd[2];
    pipe(pipefd);
    int rfd = pipefd[0];
    int wfd = pipefd[1];

    printf("fd for write end: %d\n", wfd);
    printf("fd for read end: %d\n", rfd);

    pid_t pid1 = fork();

    if (pid1 == 0) { // first child
        dup2(wfd, 1); // stdout now points to write end of pipe
        close(wfd); // close write end of pipe, since stdout now points to it
        close(rfd); // close read end of pipe bc we never needed it
        execlp("ls", "ls", NULL); // exec ls
    }

    // parent
    pid_t pid2 = fork();

    if (pid2 == 0) { // second child
        dup2(rfd, 0); // stdin now points to read end of pipe
        close(wfd); // close write end of pipe bc we never needed it
        close(rfd); // close read end of pipe, since stdin now points to it
        execlp("wc", "wc", NULL); // exec wc
    }

    close(wfd); // close write end of pipe
    close(rfd); // close read end of pipe

    // parent
    wait(NULL);
    wait(NULL);

    return 0;
}
