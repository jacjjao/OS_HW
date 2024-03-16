#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  char src_fname[100];
  char dest_fname[100];

  printf("The file's name you want to copy: ");
  scanf("%99s", src_fname);

  printf("The file's name you want to copy to: ");
  scanf("%99s", dest_fname);

  int fd_to, fd_from;
  char buf[4096];
  ssize_t nread;
  int ret;

  ret = 0;

  fd_from = open(src_fname, O_RDONLY);
  if (fd_from < 0) {
    fprintf(stderr, "Cannot open the src file\n");
    return -1;
  }

  fd_to = open(dest_fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd_to < 0) {
    fprintf(stderr, "Cannot open the dest file\n");
    ret = -1;
    goto cleanup;
  }

  while (nread = read(fd_from, buf, sizeof(buf)), nread > 0) {
    char *out_ptr = buf;
    ssize_t nwritten;

    do {
      nwritten = write(fd_to, out_ptr, nread);

      if (nwritten >= 0) {
        nread -= nwritten;
        out_ptr += nwritten;
      } else if (nwritten < 0 && errno != EINTR) {
        fprintf(stderr, "Error while writing the file\n");
        ret = -1;
        goto cleanup;
      }
    } while (nread > 0);
  }

  if (nread < 0) {
    ret = -1;
  }

cleanup:
  close(fd_from);
  if (fd_to >= 0) close(fd_to);

  printf("finish\n");

  return ret;
}