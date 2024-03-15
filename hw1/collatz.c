#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 1024
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: collatz [number]");
    return -1;
  }

  errno = 0;
  char *end;
  long num = strtol(argv[1], &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Range error occured.\n");
    return -1;
  }
  if (num <= 0) {
    fprintf(stderr, "Number should > 0\n");
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

    while (num > 1) {
      write(fd[WRITE_END], &num, sizeof(num));

      if (num % 2 == 0) {
        num /= 2;
      } else {
        num = 3 * num + 1;
      }
    }
    if (num == 1) {
      write(fd[WRITE_END], &num, sizeof(num));
    }

    close(fd[WRITE_END]);
  } else {  // parent process
    close(fd[WRITE_END]);

    long val;

    do {
      read(fd[READ_END], &val, sizeof(val));
      if (val <= 0) {
        fprintf(stderr, "Read failed\n");
        close(fd[READ_END]);
        return -1;
      }
      printf("%ld ", val);
    } while (val > 1);

    printf("\n");

    close(fd[READ_END]);
  }
}