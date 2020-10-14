#include "process_elf.h"
#include <elf.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONCAT2(X, Y) CONCAT2_WRAP(X, Y)
#define CONCAT2_WRAP(X, Y) X##Y
#define CONCAT3(X, Y, Z) CONCAT3_WRAP(X, Y, Z)
#define CONCAT3_WRAP(X, Y, Z) X##Y##Z

#define process_elf CONCAT2(process_elf, N)
#define ElfN_Ehdr CONCAT3(Elf, N, _Ehdr)
#define ElfN_Addr CONCAT3(Elf, N, _Addr)
#define ElfN_Off CONCAT3(Elf, N, _Off)
#define ElfN_Shdr CONCAT3(Elf, N, _Shdr)

#define SECTION_NAME ".GCC.command.line"

void process_elf(const char *name, FILE *fd) {
  ElfN_Ehdr header;
  if (fseek(fd, 0, SEEK_SET) != 0) {
    fprintf(stderr, "failed to set position indicator for file '%s'\n", name);
    exit(EXIT_FAILURE);
  }
  size_t read = fread(&header, 1, sizeof(ElfN_Ehdr), fd);
  if (read != sizeof(header)) {
    fprintf(stderr, "failed to read elf header from file '%s': %s\n", name,
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (fseek(fd, header.e_shoff, SEEK_SET) != 0) {
    fprintf(stderr, "failed to set position indicator for file '%s'\n", name);
    exit(EXIT_FAILURE);
  }
  size_t sh_table_size = header.e_shentsize * header.e_shnum;
  ElfN_Shdr *sh_table = malloc(sh_table_size);
  if (sh_table == NULL) {
    fprintf(stderr, "faild to allocate memory of size %zu\n", sh_table_size);
    exit(EXIT_FAILURE);
  }
  read = fread(sh_table, 1, sh_table_size, fd);
  if (read != sh_table_size) {
    fprintf(stderr, "failed to read section header table from file '%s': %s\n",
            name, strerror(errno));
    free(sh_table);
    exit(EXIT_FAILURE);
  }

  size_t str_index = 0;
  if (header.e_shstrndx != SHN_XINDEX) {
    str_index = header.e_shstrndx;
  } else {
    str_index = sh_table[0].sh_link;
  }

  const ElfN_Shdr *sh_str = &sh_table[str_index];
  const size_t str_section_size = sh_str->sh_size;
  char *str_section = malloc(str_section_size);
  if (str_section == NULL) {
    fprintf(stderr, "faild to allocate memory of size %zu\n", str_section_size);
    free(sh_table);
    exit(EXIT_FAILURE);
  }
  if (fseek(fd, sh_str->sh_offset, SEEK_SET) != 0) {
    fprintf(stderr, "failed to set position indicator for file '%s'\n", name);
    free(str_section);
    free(sh_table);
    exit(EXIT_FAILURE);
  }
  read = fread(str_section, 1, str_section_size, fd);
  if (read != str_section_size) {
    fprintf(stderr, "failed to read section name section from file '%s': %s\n",
            name, strerror(errno));
    free(str_section);
    free(sh_table);
    exit(EXIT_FAILURE);
  }

  bool found = false;
  size_t size = 0;
  size_t offset = 0;
  for (size_t i = 0; i < header.e_shnum; ++i) {
    if (strcmp(SECTION_NAME, str_section + sh_table[i].sh_name) == 0) {
      size = sh_table[i].sh_size;
      offset = sh_table[i].sh_offset;
      found = true;
    }
  }
  free(str_section);
  free(sh_table);

  if (found) {
    dump_options(name, fd, offset, size);
  } else {
    fprintf(stderr, "section '%s' not found in file '%s'\n", SECTION_NAME,
            name);
    exit(EXIT_FAILURE);
  }
}
