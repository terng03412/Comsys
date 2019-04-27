/* This example is copied from */
/* <http://www.coe.uncc.edu/~abw/parallel/pthreads/pthreads.html */
/* It illustrates the idea of shared memory programming */
/* Modification: fixed a number of bugs (typos), moved the 'watch' thread */
/* before the 'increment' thread */
/* aug-16-2000 */
/* oct-5-2000 */
/* test without mutex */

/* Comments: the behavior of this program on Solaris 2.7 is quite different */
/* from that of Red Hat Linux 6.2. On Solaris, the program acted incorrectly */
/* while the outcome on Red Hat Linux 6.2 was as expected. X.M. */

#include <pthread.h>   
#include <stdio.h>
#define MAX 10
#define MAX_COUNT 15

void * increment(int *id);
void * watch(int *id);

int count =0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t count_max = PTHREAD_COND_INITIALIZER;
int thread_id[3]  = {0,1,2};

main()
{
        int i;
        /* define the type to be pthread */
        
        pthread_t thread[3];
        /* create 3 threads*/
        pthread_create(&thread[2], NULL, (void *)watch, &thread_id[2]);
        pthread_create(&thread[0], NULL, (void *)increment, &thread_id[0]);
        pthread_create(&thread[1], NULL, (void *)increment, &thread_id[1]);
        
        for(i=0; i< 3 ; i++)
        {
                pthread_join(thread[i], NULL);
        }
}

void * watch(int *id)
{
  /* lock the variable */
  /*        pthread_mutex_lock(&count_mutex); */
  while(count <= MAX_COUNT)
    {
      /* using the condition variable for waiting for the event */
      /* pthread_cond_wait(&count_max, &count_mutex); */
      printf("Inside the watch() and the value is %d\n", count);
      fflush(stdout);
      /* yield the processor */
      if (rand() % 100 < 33)
	sleep(rand()%3);
    }
  /*unlock the variable*/
  /* pthread_mutex_unlock(&count_mutex); */
}

void * increment(int *id)
{
  int i;
  for(i=0; i< MAX ; i++)
    {
      /* lock the variable */
      /* pthread_mutex_lock(&count_mutex); */
      count++;
      printf("in increment counter by threadof id :%d, and count: %d\n",*id, count);
      fflush(stdout);
      /* for the condition notify the thread */
      /* pthread_cond_signal(&count_max); */
      /*unlock the variable*/
      /* pthread_mutex_unlock(&count_mutex); */
      /* yield the processor */
      if (rand() % 100 >= 33)
	sleep(rand()%3);
    }
}
