#ifndef SEEDCLASS_OS_COMMON_H
#define SEEDCLASS_OS_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/times.h>


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

#endif //SEEDCLASS_OS_COMMON_H
