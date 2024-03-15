#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int oshw_file_cpy(const char *src, const char *dest) {
  int fd_to, fd_from;
  char buf[4096];
  ssize_t nread;
  int saved_errno;

  fd_from = open(src, O_RDONLY);
  if (fd_from < 0) {
    fprintf(stderr, "Cannot open the src file\n");
    return -1;
  }

  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  fd_to = open(dest, O_WRONLY | O_CREAT | O_TRUNC, mode);
  if (fd_to < 0) {
    fprintf(stderr, "Cannot open the dest file\n");
    goto out_error;
  }

  while (nread = read(fd_from, buf, sizeof buf), nread > 0) {
    char *out_ptr = buf;
    ssize_t nwritten;

    do {
      nwritten = write(fd_to, out_ptr, nread);

      if (nwritten >= 0) {
        nread -= nwritten;
        out_ptr += nwritten;
      } else if (errno != EINTR) {
        fprintf(stderr, "Error while writing the file\n");
        goto out_error;
      }
    } while (nread > 0);
  }

  if (nread == 0) {
    if (close(fd_to) < 0) {
      fd_to = -1;
      fprintf(stderr, "Cannot close the dest file\n");
      goto out_error;
    }
    close(fd_from);

    return 0;
  }

out_error:
  close(fd_from);
  if (fd_to >= 0) close(fd_to);

  return -1;
}

int main(void) {
  char src_fname[100];
  char dest_fname[100];

  printf("The file's name you want to copy: ");
  scanf("%99s", src_fname);

  printf("The file's name you want to copy to: ");
  scanf("%99s", dest_fname);

  if (oshw_file_cpy(src_fname, dest_fname)) {
    printf("error while copy the file\n");
    return -1;
  }
  printf("finish\n");
  return 0;
}