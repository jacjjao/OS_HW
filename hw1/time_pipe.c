#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./time <command>\n");
    return -1;
  }

  int fd[2];
  if (pipe(fd) == -1) {
    fprintf(stderr, "Pipe failed\n");
    return -1;
  }

  pid_t pid;
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return -1;
  } else if (pid == 0) {  // child process
    close(fd[READ_END]);

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    write(fd[WRITE_END], &ts.tv_nsec, sizeof(ts.tv_nsec));

    system(argv[1]);

    close(fd[WRITE_END]);
  } else {  // parent process
    close(fd[WRITE_END]);

    wait(NULL);
    long t;
    read(fd[READ_END], &t, sizeof(t));

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    printf("%ldns elapsed\n", ts.tv_nsec - t);

    close(fd[READ_END]);
  }
}