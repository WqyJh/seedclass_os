#ifndef SEEDCLASS_OS_COMMON_H
#define SEEDCLASS_OS_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/times.h>


#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"


#define CLOCK(_CODE_) do { \
    long clktck; \
    struct tms start, end; \
    clock_t clkstart, clkend; \
    \
    clkstart = times(&start); \
    _CODE_; \
    clkend = times(&end); \
    clktck = sysconf(_SC_CLK_TCK); \
    printf("real: %fs\n",  (clkend - clkstart) / (double) clktck); \
    printf("user: %fs\n", (end.tms_utime - start.tms_utime) / (double) clktck); \
    printf("sys: %fs\n", (end.tms_stime - start.tms_stime) / (double) clktck); \
} while(0);


typedef struct
{
    uint32_t n;
    uint32_t m;
} input_t;


input_t read_input();

void write_output(uint64_t sum);

int run_sol(uint64_t (*sol)(uint32_t, uint32_t));


#endif //SEEDCLASS_OS_COMMON_H
