#include "process_elf.h"
#include <elf.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_compile_option(const char *program);
void process_elf(const char *name, FILE *fd);
char header_class(const char *name, unsigned const char *e_ident);

int main(int argc, char **argv) { print_compile_option(argv[0]); }

void print_compile_option(const char *program) {
  FILE *fd = fopen(program, "r");
  if (fd == NULL) {
    fprintf(stderr, "failed to open file '%s': %s\n", program, strerror(errno));
    exit(EXIT_FAILURE);
  }
  process_elf(program, fd);
  fclose(fd);
}

void process_elf(const char *name, FILE *fd) {
  if (fseek(fd, 0, SEEK_SET) != 0) {
    fprintf(stderr, "failed to set position indicator for file '%s'\n", name);
    exit(EXIT_FAILURE);
  }
  unsigned char e_ident[EI_NIDENT];
  size_t read = fread(&e_ident, 1, sizeof(e_ident), fd);
  if (read != sizeof(e_ident)) {
    fprintf(stderr, "failed to read elf identifier from file '%s': %s\n", name,
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  char class = header_class(name, e_ident);
  if (class == ELFCLASS32) {
    process_elf32(name, fd);
  } else if (class == ELFCLASS64) {
    process_elf64(name, fd);
  } else {
    fprintf(stderr, "invalid elf architecture number in file '%s'\n", name);
    exit(EXIT_FAILURE);
  }
}

char header_class(const char *name, unsigned const char *e_ident) {
  if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 ||
      e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3) {
    fprintf(stderr, "'%s' is not a elf file\n", name);
    return ELFCLASSNONE;
  }
  if (e_ident[EI_CLASS] != ELFCLASS32 && e_ident[EI_CLASS] != ELFCLASS64) {
    return ELFCLASSNONE;
  } else {
    return e_ident[EI_CLASS];
  }
}
