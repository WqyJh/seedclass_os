cmake_minimum_required(VERSION 3.11)
project(seedclass_os)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-O3 -Werror -Wfatal-errors -Wall")

add_executable(multisum_thread multisum_thread.c common.h)
target_link_libraries(multisum_thread pthread)

add_executable(multisum_process multisum_process.c common.h)
