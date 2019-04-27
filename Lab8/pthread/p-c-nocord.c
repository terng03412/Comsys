/* This example is rewritten based on the example from */
/* multiple threads share data with no coordination */
/* oct-03-2000 */

#include <pthread.h>   
#include <stdio.h>
#include <stdlib.h>   /* for rand() */

#define MAX 10
#define BUFLEN 64
#define MAX_COUNT 15
#define NUMTHREAD 4      /* number of threads */
#define NO_SHARE 0       /* 0 not shared among processes */

void * consumer(int *id);
void * producer(int *id);

char source[BUFLEN];
char buffer[BUFLEN];
int rCount = 0, wCount = 0;
int buflen;
int thread_id[NUMTHREAD]  = {0,1,2,3};

main()
{
  int i;
  /* define the type to be pthread */
  pthread_t thread[NUMTHREAD];

  strcpy(source,"hello+world!");
  buflen = strlen(source);


  /* create one consumer and one producer */
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
      /*      printf(" in consumer \n"); */
      /*      while(rCount < wCount) */
	{
	  /* take the char from the buffer and increment the rCount */
/*	  printf("consumed value is %c by %d\n", buffer[rCount], *id);
*/
          printf("          %c\n",buffer[rCount]);
	  rCount = (rCount + 1) % BUFLEN;
	  fflush(stdout);
	}
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
      /*      printf("in producer\n"); */
      /*      printf(" value of the full semaphore %d\n",v);*/

/*      printf("produced :%c: by  :%d:\n",buffer[wCount], *id);
*/
      strcpy(buffer,"");
      buffer[wCount] = source[wCount%buflen];
      printf("%c\n",buffer[wCount]);
      wCount = (wCount + 1) % BUFLEN;
      fflush(stdout);
      
      if (rand() % 100 < 33 )
	sleep(rand()%2);
      n ++;
    }
}
