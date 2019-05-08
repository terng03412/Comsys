#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common_threads.h"
#include "sbuf.h"

cond_t empty, fill;
mutex_t mutex;

void *producer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        Pthread_mutex_lock(&mutex);            // p1
        while (count == MAX)                   // p2
            Pthread_cond_wait(&empty, &mutex); // p3
        put(i);                                // p4
        Pthread_cond_signal(&fill);            // p5
        Pthread_mutex_unlock(&mutex);          // p6
    }
}

int loops;
void *consumer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        Pthread_mutex_lock(&mutex);           // c1
        while (count == 0)                    // c2
            Pthread_cond_wait(&fill, &mutex); // c3
        int tmp = get();                      // c4
        Pthread_cond_signal(&empty);          // c5
        Pthread_mutex_unlock(&mutex);         // c6
        printf("%d\n", tmp);
    }
}

sbuf_t *shared_data;

int main(int argc, char *argv[])
{

    pthread_t p1, p2, p3, p4;

    printf("size of sd = %lu\n", sizeof(sbuf_t));
    shared_data = malloc(sizeof(sbuf_t));
    if (shared_data == NULL)
    {
        fprintf(stderr, "Malloc unsuccessful\n");
        exit(0);
    }
    // Initialize buffer of size 5
    sbuf_init(shared_data, 5);

    Pthread_create(&p1, NULL, consumer, "1");
    Pthread_create(&p2, NULL, producer, "1");
    Pthread_create(&p3, NULL, consumer, "2");
    Pthread_create(&p4, NULL, producer, "2");

    // join waits for the threads to finish
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);
    Pthread_join(p4, NULL);

    return 0;
}
