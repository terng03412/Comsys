#ifndef __sbuf_H__
#define __sbuf_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
  int *buf;
  int *id;
  int max;   // max slot
  int front; // (front+1)%max --> first item
  int rear;  // rear%n --> last item
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} sbuf_t;

void shared_data_init(sbuf_t *sp, int n);
void shared_data_insert(sbuf_t *sp, int item, char *msg);
void shared_data_remove(sbuf_t *sp, char *msg);

#endif
