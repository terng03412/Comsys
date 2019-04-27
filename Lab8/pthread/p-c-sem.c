/* This example is rewritten based on the example from */
/* <http://www.coe.uncc.edu/~abw/parallel/pthreads/pthreads.html */
/* It illustrates the idea of shared memory programming using semaphore */
/* oct-03-2000 */

#include <pthread.h>   
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
/*
#include <sched.h>
*/

#define MAX 10
#define BUFLEN 64
#define MAX_COUNT 15
#define NUMTHREAD 4      /* number of threads */
#define NO_SHARE 0       /* 0 not shared among processes */

void * consumer(int *id);
void * producer(int *id);

char buffer[BUFLEN];
char source[BUFLEN];
int rCount = 0, wCount = 0;
int buflen;
int thread_id[NUMTHREAD]  = {0,1,2,3};
sem_t full, empty, mutex;

main()
{
  int i;
  /* define the type to be pthread */
  pthread_t thread[NUMTHREAD];

  strcpy(source,"hello-world!");
  buflen = strlen(source);

  /* initialize the semaphores */
  sem_init(&full, NO_SHARE, BUFLEN);
  sem_init(&empty, NO_SHARE, 0);
  sem_init(&mutex, NO_SHARE, 1);

  /* create two consumer and two producer */
  pthread_create(&thread[2], NULL, (void *)consumer, &thread_id[2]);
  pthread_create(&thread[3], NULL, (void *)consumer, &thread_id[3]);
  pthread_create(&thread[1], NULL, (void *)producer, &thread_id[1]);
  pthread_create(&thread[0], NULL, (void *)producer, &thread_id[0]);
  
  for(i=0; i< NUMTHREAD ; i++)
    {
      pthread_join(thread[i], NULL);
    }
}

void * consumer(int *id)
{

  int n = 0;

  while (n < 40)
    {
      /* wait for the buffer to have something in it */
      sem_wait(&empty);
      /* only one thread can access the critical region */
      sem_wait(&mutex);  
      /* take the char from the buffer and increment the rCount */
      printf("          %c\n",buffer[rCount]);
      rCount = (rCount + 1) % BUFLEN;
      fflush(stdout);
      /* release the mutex */
      sem_post(&mutex);
      
      /* signal the producer that the buffer has been consumed */
      sem_post(&full);
      if (rand() % 100 >= 33)
	sleep(rand()%3);
      n ++;
    }
}

void * producer(int *id)
{

  int v;
  int n = 0;

  while (n < 40)
    {
      /* wait for the buffer to have space */
      sem_wait(&full);
      /* sem_getvalue(&full, &v);*/
      /* only one thread can access the critical region */
      sem_wait(&mutex);
      strcpy(buffer,"");
      buffer[wCount] = source[wCount%buflen];
      printf("%c\n",buffer[wCount]);
      wCount = (wCount + 1) % BUFLEN;
      fflush(stdout);
      
      /* release the mutex */
      sem_post(&mutex);
      
      /* signal the consumer that the buffer has been filled */
      sem_post(&empty);
      /* sem_getvalue(&empty, &v); */
      if (rand() % 100 < 33)
	sleep(rand()%2);
      n ++;
    }
}
