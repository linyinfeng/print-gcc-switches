#ifndef PRINT_GCC_OPTIONS_PROCESS_ELF_H_INCLUDED
#define PRINT_GCC_OPTIONS_PROCESS_ELF_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

extern void process_elf64(const char *name, FILE *fd);
extern void process_elf32(const char *name, FILE *fd);
extern void dump_options(const char *name, FILE *fd, size_t offset,
                         size_t size);

#endif
