#ifndef __SBUF_H__
#define __SBUF_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* $begin sbuft */
typedef struct
{
  int *buf;              /* Buffer array */
  int *id;               /* Thread number array */
  int max;               /* Maximum number of slots */
  int front;             /* buf[(front+1)%n] is first item */
  int rear;              /* buf[rear%n] is last item */
  pthread_mutex_t mutex; /* Protects accesses to buf */
  pthread_mutex_t slots; /* Counts available slots */
  pthread_mutex_t items; /* Counts available items */
  pthread_mutex_t cond;
} sbuf_t;

void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item, char *msg);
int sbuf_remove(sbuf_t *sp, char *msg);

#endif /* __SBUF_H__ */
