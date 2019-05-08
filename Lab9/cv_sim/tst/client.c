#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "sbuf.h"

sbuf_t *shared_data;
// pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
// pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
// pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg)
{
    char *msg = arg;
    int item = 1;
    while (1)
    {
        Pthread_mutex_lock(&(shared_data->mutex));
        //check shared_data is full or not if(full)? wait:continue
        while (shared_data->rear - shared_data->front >= shared_data->max)
        {
            Pthread_cond_wait(&(shared_data->cond), &(shared_data->mutex));
        }
        // insert item
        sbuf_insert(shared_data, item, msg);
        item++;
        Pthread_cond_signal(&(shared_data->cond)); // signal to sleep thread
        Pthread_mutex_unlock(&(shared_data->mutex));
    }

    return NULL;
}

void *consumer(void *arg)
{
    char *msg = arg;
    while (1)
    {
        Pthread_mutex_lock(&(shared_data->mutex));
        //check shared_data is empty or not if(empty)? wait:continue
        while (shared_data->rear - shared_data->front <= 0)
        {
            Pthread_cond_wait(&(shared_data->cond), &(shared_data->mutex));
        }
        // remove item shared_data
        sbuf_remove(shared_data, msg);
        Pthread_cond_signal(&(shared_data->cond)); // signal to sleep thread
        Pthread_mutex_unlock(&(shared_data->mutex));
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i;
    int consumers = 4;
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
