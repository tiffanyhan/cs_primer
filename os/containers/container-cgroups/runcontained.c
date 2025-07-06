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
#include <sys/syscall.h>

#define STACK_SIZE 65536

struct command_config {
  int argc;
  char **argv;
};

int noopchild(void *arg) {
  return 0;
}


int main(int argc, char**argv) {
  struct command_config config = {0};
  int flags = 0;
  pid_t child_pid = 0;
  pid_t pid = getpid();
  printf("parent pid: %d\n", pid);

  // Configure cgroup
  mkdir("/sys/fs/cgroup/mycgroup", 0755);

  FILE *procs_fp = fopen("/sys/fs/cgroup/mycgroup/cgroup.procs", "w");
  if (procs_fp) {
    fprintf(procs_fp, "%d\n", getpid());
    fclose(procs_fp);
  }

  FILE *pids_fp = fopen("/sys/fs/cgroup/mycgroup/pids.max", "w");
  if (pids_fp) {
    fprintf(pids_fp, "10\n");
    fclose(pids_fp);
  }

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

  printf("Cloning 15 times\n");

  // Clone parent many times
  for (int i = 0; i < 15; i++) {
    if ((child_pid = clone(noopchild, stack + STACK_SIZE, flags, &config)) == -1) {
      fprintf(stderr, "Clone failed\n");
      perror("cloning 15 times\n");
      exit(2);
    }
  }
  wait(NULL);
  printf("Cloning many times succeeded\n");

  return 0;
}
