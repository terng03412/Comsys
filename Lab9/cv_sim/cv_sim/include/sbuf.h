#ifndef __SBUF_H__
#define __SBUF_H__

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
  int back;  // rear%n --> last item
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} sbuf;

void queue_init(sbuf *sp, int n);
void queue_insert(sbuf *sp, int item, char *msg);
void queue_remove(sbuf *sp, char *msg);

#endif
