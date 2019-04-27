/* This example is rewritten based on the example from */
/* <http://www.coe.uncc.edu/~abw/parallel/pthreads/pthreads.html */
/* It illustrates the idea of shared memory programming */
/* It simulates the access to balance of a bank account from multiple */
/* threads. Demonstrate the importance of proper coordination. */
/* oct-09-2000 */

#include <pthread.h>   
#include <stdio.h>
#define MAX 20
#define BUFLEN 24
#define MAX_COUNT 15
#define NUMTHREAD 3      /* number of threads */
#define LIMIT 50         /* low balance limit */

void * depositor(int *id);
void * withdrawer(int *id);

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t belowLimit  = PTHREAD_COND_INITIALIZER;
int thread_id[NUMTHREAD]  = {0,1,2};
int i = 0, j = 0;

/* shared variable */
double balance = 0;

main()
{
  int i;
  /* define the type to be pthread */
  pthread_t thread[NUMTHREAD];

  /* create 2 threads*/
  /* create one depositor and one withdrawer */
  pthread_create(&thread[1], NULL, (void *)withdrawer, &thread_id[1]);
  pthread_create(&thread[2], NULL, (void *)withdrawer, &thread_id[2]);
  pthread_create(&thread[0], NULL, (void *)depositor, &thread_id[0]);
  
  for(i=0; i< NUMTHREAD ; i++)
    {
      pthread_join(thread[i], NULL);
    }
}

void * depositor(int *id)
{
  double deposit;

  while(j < MAX)
    {
      deposit = (double)(rand() % 100);/* generate a random amt of deposit?*/
      /* lock the variable */
      /*      pthread_mutex_lock(&count_mutex); */
      balance += deposit;
      printf("          deposited :%10.2f balance is %10.2f: by %d\n", deposit, balance, *id);
      fflush(stdout);
      j ++;
      /*      if (balance >= LIMIT)
	pthread_cond_signal(&belowLimit);
      pthread_mutex_unlock(&count_mutex);
      */
      if (rand()%100 >= 50)
	sleep(rand()%2);
    }
}

void * withdrawer(int *id)
{

  double debt;

  /* lock the variable */
  /*  pthread_mutex_lock(&count_mutex);*/
  while (i < MAX)
    {
      /*      if (balance < LIMIT) */
      /* wait for the balance to have enough money */
      /*      pthread_cond_wait(&belowLimit, &count_mutex);
      */
      debt = (double)(rand() % 60);  /* generate a random amt of withdraw */
      if (balance - debt < LIMIT)
	printf("sorry can't withdraw\n");
      else
	{
	  balance -= debt;
	  printf("%d withdraw %10.2f : by  :%d: balance %10.2f\n",i, debt, *id, balance);
	}
      fflush(stdout);
      i ++;
      if (rand()%100 >= 50)
	sleep(rand()%2);
    }
  /*unlock the variable*/
  /*  pthread_mutex_unlock(&count_mutex); */
}
