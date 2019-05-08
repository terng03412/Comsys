#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"

sbuf_t *shared_data;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int *buffer;
int max = 6;
int use_ptr = 0;
int fill_ptr = 0;
int num_full = 0;
int consumers = 1;

void *producer(void *arg)
{
    char *msg = arg;
    int item = 1;
    while (1)
    {
        Mutex_lock(&m);
        while (num_full == max)
            Cond_wait(&empty, &m);
        sbuf_insert(shared_data, item, msg);
        item++;
        if (item % max == 0)
            item = 1;

        Cond_signal(&fill);
        Mutex_unlock(&m);
    }

    for (i = 0; i < consumers; i++)
    {
        Mutex_lock(&m);
        while (num_full == max)
            Cond_wait(&empty, &m);
        sbuf_insert(shared_data, -1, msg);
        Cond_signal(&fill);
        Mutex_unlock(&m);
    }

    return NULL;
}

void *consumer(void *arg)
{
    char *msg = arg;
    int item;
    while (1)
    {
        Mutex_lock(&m);
        while (num_full == 0)
            Cond_wait(&fill, &m);

        item = sbuf_remove(shared_data, msg);
        Cond_signal(&empty);
        Mutex_unlock(&m);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i;
    consumers = 4;
    printf("size of sd = %lu\n", sizeof(sbuf_t));
    shared_data = malloc(sizeof(sbuf_t));
    if (shared_data == NULL)
    {
        fprintf(stderr, "Malloc unsuccessful\n");
        exit(0);
    }
    // Initialize buffer of size 5
    sbuf_init(shared_data, 5);

    pthread_t pid, cid[consumers];

    Pthread_create(&pid, NULL, producer, NULL);
    for (i = 0; i < consumers; i++)
    {
        Pthread_create(&cid[i], NULL, consumer, (void *)(long long int)i);
    }
    Pthread_join(pid, NULL);
    for (i = 0; i < consumers; i++)
    {
        Pthread_join(cid[i], NULL);
    }

    return 0;
}
