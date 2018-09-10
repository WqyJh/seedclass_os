#include "common.h"

#include <stdio.h>
#include <stdlib.h>


input_t read_input()
{
    input_t in = {0, 0};
    FILE *f = NULL;

    f = fopen(INPUT_FILE, "r");

    if (f == NULL) {
        perror("fopen() failed");
        goto EXIT;
    }

    fscanf(f, "N=%u\n", &in.n);
    fscanf(f, "M=%u", &in.m);

EXIT:
    if (f) {
        fclose(f);
        f = NULL;
    }
    return in;
}


void write_output(uint64_t sum)
{
    FILE *f = NULL;

    f = fopen(OUTPUT_FILE, "w");

    if (f == NULL) {
        perror("fopen() failed");
        goto EXIT;
    }

    fprintf(f, "%lu\n", sum);

EXIT:
    if (f) {
        fclose(f);
    }
    return;
}


int run_sol(uint64_t (*sol)(uint32_t, uint32_t))
{
    int ret = EXIT_FAILURE;
    uint64_t sum = 0;
    input_t in;

    in = read_input();

    if (in.n == 0) {
        fprintf(stderr, "input error: n=%u, m=%u\n", in.n, in.m);
        goto EXIT;
    }

    CLOCK(
            sum = sol(in.n, in.m);
    );

    write_output(sum);

    ret = EXIT_SUCCESS;

EXIT:
    return ret;
}