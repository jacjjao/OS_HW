#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./time <command>\n");
    return -1;
  }
  char *cmd = argv[1];
}