#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>   // for O_WRONLY, open()

#define SLEEP_SEC 3
#define NUM_MULS 100000000
#define NUM_MALLOCS 100000
#define MALLOC_SIZE 1000

// For struct timeval (microsecond precision)
double elapsed_timeval(struct timeval start, struct timeval end) {
    double sec = (double)(end.tv_sec - start.tv_sec);
    double usec = (double)(end.tv_usec - start.tv_usec) / 1e6;
    return sec + usec;
}

// TODO define this struct
struct profile_times {
  pid_t pid;

  struct timeval start_time;
  struct timeval end_time;

  struct timeval start_utime;
  struct timeval end_utime;

  struct timeval start_stime;
  struct timeval end_stime;
};

// TODO populate the given struct with starting information
void profile_start(struct profile_times *t) {
  t->pid = getpid();
  gettimeofday(&t->start_time, NULL);
  
  struct rusage start_rusage;
  getrusage(RUSAGE_SELF, &start_rusage);
  t->start_utime = start_rusage.ru_utime;
  t->start_stime = start_rusage.ru_stime;
}

// TODO given starting information, compute and log differences to now
void profile_log(struct profile_times *t) {
  gettimeofday(&t->end_time, NULL);

  struct rusage end_rusage;
  getrusage(RUSAGE_SELF, &end_rusage);
  t->end_utime = end_rusage.ru_utime;
  t->end_stime = end_rusage.ru_stime;

  double elapsed = elapsed_timeval(t->start_time, t->end_time);
  double elapsed_user = elapsed_timeval(t->start_utime, t->end_utime);
  double elapsed_sys = elapsed_timeval(t->start_stime, t->end_stime);
  
  printf("[pid %d] real: %.3fs user: %.3fs system: %.3fs\n", t->pid, elapsed, elapsed_user, elapsed_sys);
}

int main(int argc, char *argv[]) {
  struct profile_times t;

  // TODO profile doing a bunch of floating point muls
  float x = 1.0;
  profile_start(&t);
  for (int i = 0; i < NUM_MULS; i++)
    x *= 1.1;
  profile_log(&t);

  // TODO profile doing a bunch of mallocs
  profile_start(&t);
  void *p;
  for (int i = 0; i < NUM_MALLOCS; i++)
    p = malloc(MALLOC_SIZE);
  profile_log(&t);

  // TODO profile sleeping
  profile_start(&t);
  sleep(SLEEP_SEC);
  profile_log(&t);

  // profile system call
  int fd = open("/dev/null", O_WRONLY);
  char buf[1024] = {0};

  // profile writing to a file
  profile_start(&t);
  for (int i = 0; i < 100000; i++) {
      write(fd, buf, sizeof(buf));
  }
  profile_log(&t);

  close(fd);
}
