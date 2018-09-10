cmake_minimum_required(VERSION 3.11)
project(seedclass_os)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-O3 -Werror -Wfatal-errors -Wall -Wextra -D_XOPEN_SOURCE")

add_executable(multisum_thread thread/multisum_thread.c thread/common.h thread/common.c)
target_link_libraries(multisum_thread pthread)

add_executable(multisum_process process/multisum_process.c thread/common.h thread/common.c)

configure_file(input.txt ${CMAKE_BINARY_DIR}/)