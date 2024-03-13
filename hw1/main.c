#include "filecpy.h"
#include <stdio.h>

int main() {
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