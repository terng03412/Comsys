/* cc thisfile.c -lthread -lpthread */
#define _REENTRANT    /* basic 3-lines for threads */
#include <pthread.h>

#define NUM_THREADS 5
#define SLEEP_TIME 10
void *sleeping(void *);   /* thread routine */
int i;
pthread_t tid[NUM_THREADS];      /* array of thread IDs */

int
main(int argc, char *argv[])
{
       for ( i = 0; i < NUM_THREADS; i++)
            pthread_create(&tid[i], NULL, sleeping,
	                   (void *)SLEEP_TIME);
	    for ( i = 0; i < NUM_THREADS; i++)
		pthread_join(tid[i], NULL);
	printf("main() reporting that all %d threads have terminated\n", i);
	return (0);
}  /* main */

void *
sleeping(void *arg)
{
    int sleep_time = (int)arg;
    printf("thread %d sleeping %d seconds ...\n", pthread_self(), sleep_time);
    sleep(sleep_time);
    printf("\nthread %d awakening\n", pthread_self());
    return (NULL);
}
