#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>

#define SHM_SIZE 64
#define PROJ_ID 2333
#define SHM_NAME "/dev/shm/seedclass_os_wqy"


key_t g_key = -1;
int g_shmid = -1;
uint64_t *g_shm_sum = NULL;


static int worker(uint32_t n, uint32_t start)
{
    uint64_t _sum = start;

    for (uint32_t i = 1; i < n; ++i) {
        _sum += i + start;
    }

    __sync_add_and_fetch(g_shm_sum, _sum);

    return 0;
}


void _sol(uint32_t n, uint32_t m)
{
    pid_t *processes = NULL;
    uint32_t n_per_thread;
    uint32_t _start = 0;
    uint32_t _n = 0;

    g_key = ftok(SHM_NAME, PROJ_ID);

    if (g_key == -1) {
        fprintf(stderr, "failed to create key\n");
        goto EXIT;
    }

    g_shmid = shmget(g_key, SHM_SIZE, 0666 | IPC_CREAT);

    if (g_shmid == -1) {
        fprintf(stderr, "shm_create() failed");
        goto EXIT;
    }

    g_shm_sum = shmat(g_shmid, 0, 0);

    if ((void *) -1 == g_shm_sum) {
        perror("shmat() failed");
        g_shm_sum = NULL;
        goto EXIT;
    }

    *g_shm_sum = 0;

    n_per_thread = m / n;

    processes = malloc(n * sizeof(pid_t));

    if (processes == NULL) {
        perror("malloc() failed");
        goto EXIT;
    }

    _start = 1;
    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            _start = 1;
            _n = m - n_per_thread * (n - 1);
        } else {
            _start += _n;
            _n = n_per_thread;
        }

        processes[i] = fork();

        if (processes[i] == -1) {
            perror("fork() failed");
            goto EXIT;
        }

        if (processes[i] == 0) {
            exit(worker(_n, _start));
        }
    }

    for (int i = 0; i < n; ++i) {
        waitpid(processes[i], NULL, 0);
    }

    printf("sum: %lu\n", *g_shm_sum);

EXIT:
    if (processes) {
        free(processes);
        processes = NULL;
    }

    if (g_shm_sum) {
        shmdt(g_shm_sum);
        g_shm_sum = NULL;
    }

    if (g_shmid > 0) {
        shmctl(g_shmid, IPC_RMID, NULL);
        g_shmid = -1;
    }

    return;
}


int main()
{
    uint32_t n = 100;
    uint32_t m = 0xFFFFFFFF;
    int ret = EXIT_FAILURE;

    {
        int fd = -1;

        if ((fd = open(SHM_NAME, O_CREAT | O_RDONLY)) == -1) {
            perror("open() failed");
            goto EXIT;
        }

        close(fd);
    }

    CLOCK(_sol(n, m));

    ret = EXIT_SUCCESS;

EXIT:
    return ret;
}