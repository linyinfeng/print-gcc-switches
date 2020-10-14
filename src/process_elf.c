#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump_options(const char *name, FILE *fd, size_t offset, size_t size) {
  if (fseek(fd, offset, SEEK_SET) != 0) {
    fprintf(stderr, "failed to set position indicator for file '%s'\n", name);
    exit(EXIT_FAILURE);
  }
  char *buffer = malloc(size);
  if (buffer == NULL) {
    fprintf(stderr, "faild to allocate memory of size %zu\n", size);
    exit(EXIT_FAILURE);
  }
  size_t read = fread(buffer, 1, size, fd);
  if (read != size) {
    fprintf(stderr,
            "failed to read compiler options section from file '%s': %s\n",
            name, strerror(errno));
    free(buffer);
    exit(EXIT_FAILURE);
  }

  printf("compile options:\n");
  bool newline = false;
  for (size_t i = 0; i < size; ++i) {
    if (i == 0 || newline) {
      putchar('\t');
      newline = false;
    }

    char c = buffer[i];
    if (c == '\0') {
      putchar('\n');
      newline = true;
    } else {
      putchar(c);
    }
  }

  free(buffer);
}

#define N 32
#include "process_elf_impl.c"

#undef N
#define N 64
#include "process_elf_impl.c"
