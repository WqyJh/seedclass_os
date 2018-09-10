#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


uint64_t sum = 0;


typedef union
{
    struct
    {
        uint32_t start;
        uint32_t n;
    } data;

    uint64_t data_un;
} arg_t;


void *sol1_worker(void *args)
{
    arg_t a;
    a.data_un = (uint64_t) args;

    for (uint32_t i = 0; i < a.data.n; ++i) {
        __sync_add_and_fetch(&sum, i + a.data.start);
    }

    return NULL;
}


void *sol2_worker(void *args)
{
    arg_t a;
    a.data_un = (uint64_t) args;

    uint64_t _sum = a.data.start;

    for (uint32_t i = 1; i < a.data.n; ++i) {
        _sum += i + a.data.start;
    }

    __sync_add_and_fetch(&sum, _sum);

    return NULL;
}


void _sol(uint32_t n, uint32_t m, void *(*worker)(void *args))
{
    sum = 0;
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
    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            arg.data.start = 1;
            arg.data.n = m - n_per_thread * (n - 1);
        } else {
            arg.data.start += arg.data.n;
            arg.data.n = n_per_thread;
        }

        pthread_create(&threads[i], NULL, worker, (void *) arg.data_un);
    }

    for (int i = 0; i < n; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("sum: %lu\n", sum);

EXIT:
    if (threads) {
        free(threads);
        threads = NULL;
    }

    return;
}


#define sol1(n, m) _sol(n, m, sol1_worker)

#define sol2(n, m) _sol(n, m, sol2_worker)


int main()
{
    uint32_t n = 100;
    uint32_t m = 0xFFFFFFFF;

    CLOCK(sol2(n, m));

//    CLOCK(sol1(n, m));

    return 0;
}