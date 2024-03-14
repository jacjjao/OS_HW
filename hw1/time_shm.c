#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  const char *SHM_NAME = "OSHW_TIME";
  const int SIZE = 1024;

  if (argc != 2) {
    fprintf(stderr, "Usage: ./time <command>\n");
    return -1;
  }

  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, SIZE);

  pid_t pid;
  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork Failed");
    return 1;
  } else if (pid == 0) { // child process
    void *ptr = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    sprintf(ptr, "%ld", ts.tv_nsec);

    system(argv[1]);
  } else { // parent process
    void *ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    wait(NULL);

    struct timespec now;
    timespec_get(&now, TIME_UTC);

    time_t t;
    sscanf(ptr, "%ld", &t);

    printf("%ldns elapsed\n", now.tv_nsec - t);
  }
}