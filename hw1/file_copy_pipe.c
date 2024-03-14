#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define SIZE 1024

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: <In file> <Out file>\n");
    return -1;
  }

  int in_f;
  in_f = open(argv[1], O_RDONLY);
  if (in_f < 0) {
    fprintf(stderr, "Cannot open the input file\n");
    return -1;
  }
  int out_f;
  out_f = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (out_f < 0) {
    fprintf(stderr, "Cannot open the copy file\n");
    close(in_f);
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
    close(fd[WRITE_END]);

    ssize_t byte_read;
    char buf[SIZE];
    while (byte_read = read(fd[READ_END], buf, SIZE), byte_read > 0) {
      write(out_f, buf, byte_read);
    }

    close(out_f);
    close(fd[READ_END]);

    if (byte_read < 0) {
      fprintf(stderr, "Read pipe failed\n");
      return -1;
    }
  } else {  // parent process
    close(fd[READ_END]);

    ssize_t byte_read;
    char buf[SIZE];
    while (byte_read = read(in_f, buf, SIZE), byte_read > 0) {
      write(fd[WRITE_END], buf, byte_read);
    }

    close(in_f);
    close(fd[WRITE_END]);

    wait(NULL);

    if (byte_read < 0) {
      fprintf(stderr, "Read input file failed\n");
      return -1;
    }
    printf("Finish\n");
  }
}