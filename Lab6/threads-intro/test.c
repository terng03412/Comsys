#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"
#include "common_threads.h"

void *task(void *arg)
{
    pid_t pid = getpid();
    printf("My pid is %d\n", pid);
    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, &task, NULL);
    task(NULL);
    return 0;
}
