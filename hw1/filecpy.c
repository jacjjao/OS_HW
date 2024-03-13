#include "filecpy.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int oshw_file_cpy(const char *src, const char *dest) {
  int fd_to, fd_from;
  char buf[4096];
  ssize_t nread;
  int saved_errno;

  fd_from = open(src, O_RDONLY);
  if (fd_from < 0)
    return -1;

  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  fd_to = open(dest, O_WRONLY | O_CREAT | O_TRUNC, mode);
  if (fd_to < 0)
    goto out_error;

  while (nread = read(fd_from, buf, sizeof buf), nread > 0) {
    char *out_ptr = buf;
    ssize_t nwritten;

    do {
      nwritten = write(fd_to, out_ptr, nread);

      if (nwritten >= 0) {
        nread -= nwritten;
        out_ptr += nwritten;
      } else if (errno != EINTR) {
        goto out_error;
      }
    } while (nread > 0);
  }

  if (nread == 0) {
    if (close(fd_to) < 0) {
      fd_to = -1;
      goto out_error;
    }
    close(fd_from);

    return 0;
  }

out_error:
  close(fd_from);
  if (fd_to >= 0)
    close(fd_to);

  return -1;
}