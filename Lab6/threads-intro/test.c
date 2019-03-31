#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"
#include "common_threads.h"

void *worker(void *arg)
{
    printf("Worker\n");
    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, &worker, NULL);
    pthread_yield_np();
    printf("Main\n");
    return 0;
}
