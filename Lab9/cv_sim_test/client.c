#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common_threads.h"
#include "sbuf.h"

sbuf *shared_data;

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
		shared_data_insert(shared_data, item, msg);
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
		shared_data_remove(shared_data, msg);
		Pthread_cond_signal(&(shared_data->cond)); // signal to sleep thread
		Pthread_mutex_unlock(&(shared_data->mutex));
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int consumers = 4;
	// pthread_t c[consumers];

	pthread_t p1, p2, p3, p4;

	printf("size of sd = %lu\n", sizeof(sbuf));
	shared_data = malloc(sizeof(sbuf));
	if (shared_data == NULL)
	{
		fprintf(stderr, "Malloc unsuccessful\n");
		exit(0);
	}
	// init max size
	shared_data_init(shared_data, 5);

	Pthread_create(&p1, NULL, consumer, "1");
	Pthread_create(&p2, NULL, producer, "1");
	Pthread_create(&p3, NULL, consumer, "2");
	Pthread_create(&p4, NULL, producer, "2");

	// join thread after finish
	Pthread_join(p1, NULL);
	Pthread_join(p2, NULL);
	Pthread_join(p3, NULL);
	Pthread_join(p4, NULL);

	return 0;
}
