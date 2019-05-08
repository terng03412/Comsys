/* 
 * goodcnt.c - A correctly synchronized counter program 
 */
/* $begin goodcnt */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

#include "common_threads.h"

void *thread(void *vargp); /* Thread routine prototype */

/* Global shared variables */
/* $begin goodcntsemdef */
volatile long cnt = 0; /* Counter */

#ifdef __APPLE__
dispatch_semaphore_t mutex; /* Semaphore that protects counter */
#else
sem_t mutex;
#endif
/* $end goodcntsemdef */

int main(int argc, char **argv)
{
    int niters;
    int num_thread = 10;
    // pthread_t tid[0], tid[1];
    pthread_t tid[num_thread];

    /* Check input argument */
    if (argc != 2)
    {
        printf("usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);

/* Create threads and wait for them to finish */
/* $begin goodcntseminit */
#ifdef __APPLE__
    mutex = dispatch_semaphore_create(1); // init with value of 1
#else
    Sem_init(&mutex, 0, 1); /* mutex = 1 */
                            /* $end goodcntseminit */
#endif

    for (int j = 0; j < num_thread; j++)
    {
        Pthread_create(&tid[j], NULL, thread, &j);
        Pthread_join(tid[j], NULL);
    }

    /* Check result */
    if (cnt != (num_thread * niters))
        printf("BOOM! cnt=%ld\n", cnt);
    else
        printf("OK cnt=%ld\n", cnt);
    exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
    int tid = *((int *)vargp);
    printf("tid = %d \n", tid);
    int i, niters = *((int *)vargp);
    static int numt = 0;
    printf("thread num = %d doing %d iterations\n", ++numt, niters);

    /* $begin goodcntthread */
    for (i = 0; i < niters; i++)
    {
#ifdef __APPLE__
        dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);
#else
        P(&mutex);
#endif
        cnt++;
#ifdef __APPLE__
        dispatch_semaphore_signal(mutex);
#else
        V(&mutex);
#endif
    }
    /* $end goodcntthread */
    return NULL;
}
/* $end goodcnt */
