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

struct child_config {
  int argc;
  char **argv;
};

/* Entry point for child after `clone` */
int child(void *arg) {
  struct child_config *config = arg;
  char *envp[] = { "PATH=/bin:/usr/bin", NULL };

  // pivot root
  const char *new_root = "/new_root";
  const char *put_old = "/new_root/old_root";

  if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) < 0) {
    perror("make old root private");
    exit(EXIT_FAILURE);
  }

  if (mount(new_root, new_root, NULL, MS_BIND | MS_REC, NULL) < 0) {
    perror("bind mount new root");
    exit(EXIT_FAILURE);
  }

  if (mkdir(put_old, 0755) < 0 && errno != EEXIST) {
    perror("make dir put_old");
    exit(EXIT_FAILURE);
  }

  if (syscall(SYS_pivot_root, new_root, put_old) < 0) {
    perror("pivot_root");
    exit(EXIT_FAILURE);
  }

  if (chdir("/") < 0) {
    perror("change dir to new root");
    exit(EXIT_FAILURE);
  }

  if (umount2("/old_root", MNT_DETACH) < 0) {
    perror("unmount the old root");
    exit(EXIT_FAILURE);
  }

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

  // Allocate stack for child
  char *stack = 0;
  if (!(stack = malloc(STACK_SIZE))) {
    fprintf(stderr, "Malloc failed");
    exit(1);
  }

  flags = flags | SIGCHLD | CLONE_NEWNET | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | CLONE_NEWNS;

  // Clone parent, enter child code
  if ((child_pid = clone(child, stack + STACK_SIZE, flags, &config)) == -1) {
    fprintf(stderr, "Clone failed");
    exit(2);
  }

  wait(NULL);
  
  return 0;
}
