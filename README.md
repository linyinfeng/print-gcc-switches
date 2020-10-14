# print-gcc-switches

This is just a demo printing the compile options with which it was built on Linux.

GCC is required, Clang does not implement the gcc option `-frecord-gcc-switches`, which tell the linker to record compile options in ELF section `.GCC.command.line` of binary.

## Usage

```sh
cmake DEMO_ROOT
make
./print-gcc-switches
```
