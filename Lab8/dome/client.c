#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common_threads.h"
#include "smoker.h"

smoker_t *shared_data;

void *agent(void *arg)
{
  char *msg = arg;
  while(true){
    smoker_insert(shared_data);
  }
  return NULL;
}  

void *smoker(void *arg)
{
  char *msg = arg;
  while(true){
    smoker_remove(shared_data,msg);
  }
  return NULL;
}

int main(int argc, char *argv[])
{

  pthread_t p1, p2, p3, p4;

  printf("size of sd = %lu\n", sizeof(smoker_t));
  shared_data = malloc(sizeof(smoker_t));
  if (shared_data == NULL) {
    fprintf(stderr, "Malloc unsuccessful\n");
    exit(0);
  }
  // Initialize buffer of size 5
  smoker_init(shared_data);
  
  Pthread_create(&p1, NULL, agent, 0);    // thread agent
  Pthread_create(&p2, NULL, smoker, "p"); // thread paper
  Pthread_create(&p3, NULL, smoker, "m"); // thread match
  Pthread_create(&p4, NULL, smoker, "t"); // thread tabacco

  // join waits for the threads to finish
  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  Pthread_join(p3, NULL);
  Pthread_join(p4, NULL);

  return 0;
}
