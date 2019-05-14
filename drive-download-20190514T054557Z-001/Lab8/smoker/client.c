#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common_threads.h"
#include "smoker.h"

table_t *shared_table;

// Agent Thread to place item
void *agent(void *arg)
{
  while (1) {
    place_item(shared_table);
    sleep(1);
  }
  return NULL;
}  

// Smoker Thread to take item
void *smoker(void *arg)
{
  // Different item number -> different type of smoker
  int item = (int)arg;
  /*
    0 -> Tobacco Smoker (lacks Paper and Match)
    1 -> Paper Smoker (lacks Tobacco and Match)
    2 -> Match Smoker (lacks Tobacco and Paper )
  */
  while (1) {
    take_item(shared_table, item);
    sleep(1);
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t p1, p2, p3, p4;

  printf("size of table = %lu\n", sizeof(table_t));
  shared_table = malloc(sizeof(table_t));
  if (shared_table == NULL) {
    fprintf(stderr, "Malloc unsuccessful\n");
    exit(0);
  }

  // Initialize table start with Agent place 2 random items
  table_init(shared_table);
  
  // Agent Thread
  Pthread_create(&p1, NULL, agent, 4);

  // Smoker Thread
  // Tobacco Holder Thread
  Pthread_create(&p2, NULL, smoker, 0);
  // Paper Holder Thread
  Pthread_create(&p3, NULL, smoker, 1);
  // Match Holder Thread
  Pthread_create(&p4, NULL, smoker, 2);

  // join waits for the threads to finish
  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  Pthread_join(p3, NULL);
  Pthread_join(p4, NULL);

  return 0;
}
