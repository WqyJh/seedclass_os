#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static uint64_t s_sum = 0;


typedef union
{
    struct
    {
        uint32_t start;
        uint32_t n;
    } data;

    uint64_t data_un;
} arg_t;


static void *child_worker(void *args)
{
    arg_t a;
    a.data_un = (uint64_t) args;

    uint64_t _sum = a.data.start;

    for (uint32_t i = 1; i < a.data.n; ++i) {
        _sum += i + a.data.start;
    }

    __sync_add_and_fetch(&s_sum, _sum);

    return NULL;
}


static uint64_t sol_thread(uint32_t n, uint32_t m)
{
    s_sum = 0;
    arg_t arg;
    pthread_t *threads = NULL;
    uint32_t n_per_thread;

    n_per_thread = m / n;

    threads = malloc(n * sizeof(pthread_t));

    if (threads == NULL) {
        perror("malloc() failed");
        goto EXIT;
    }

    arg.data.start = 1;
    for (uint32_t i = 0; i < n; ++i) {
        if (i == 0) {
            arg.data.start = 1;
            arg.data.n = m - n_per_thread * (n - 1);
        } else {
            arg.data.start += arg.data.n;
            arg.data.n = n_per_thread;
        }

        pthread_create(&threads[i], NULL, child_worker, (void *) arg.data_un);
    }

    for (uint32_t i = 0; i < n; ++i) {
        pthread_join(threads[i], NULL);
    }

EXIT:
    if (threads) {
        free(threads);
        threads = NULL;
    }

    return s_sum;
}


int main()
{
    return run_sol(sol_thread);
}