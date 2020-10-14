cmake_minimum_required(VERSION 3.0)
project(print-gcc-switches)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(print-gcc-switches
    src/main.c
    src/process_elf.c)
target_compile_options(print-gcc-switches PUBLIC
    -Wall
    -frecord-gcc-switches)
install(TARGETS print-gcc-switches)
