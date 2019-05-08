#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "common_threads.h"
#include "sbuf.h"

void shared_data_init(sbuf *sp, int n)
{
  void *p;
  assert((p = malloc(sizeof(int))) != NULL);
  sp->buf = p;
  assert((p = malloc(sizeof(char))) != NULL);
  sp->id = p;
  sp->max = n; // init max size
  sp->front = 0;
  sp->back = 0; // start shared_data
}

void shared_data_insert(sbuf *sp, int item, char *num)
{
  int idx;
  idx = (sp->back + 1) % (sp->max);
  sp->back++;
  sp->buf[idx] = item; // insert
  sp->id[idx] = num[0];
  int remain = sp->back - sp->front;

  printf("I am a PRODUCER # %c inserting item %d from PRODUCER # %c\n", sp->id[idx], item, sp->id[idx]);
  printf("Number of items in shared_data now is %d\n", remain);
  sleep(1);
}
void shared_data_remove(sbuf *sp, char *num)
{
  int item, idx;
  idx = (sp->front + 1) % (sp->max);
  sp->front++;
  item = sp->buf[idx]; // remove
  int remain = sp->back - sp->front;
  printf("I am a consumer # %c consuming item %d from producer # %c\n", num[0], item, sp->id[idx]);
  printf("Number of items in shared_data now is %d\n", remain);
  sleep(1);
}
