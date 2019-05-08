#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "common_threads.h"
#include "smoker.h"
void smoker_init(smoker_t *sp)
{
    Sem_init(&sp->agent, 0, 1); // set agen for ready
    Sem_init(&sp->share, 0, 1);
    Sem_init(&sp->tobacco, 0, 0); // set all smoker 
    Sem_init(&sp->paper, 0, 0);
    Sem_init(&sp->match, 0, 0);
}
void smoker_insert(smoker_t *sp){
    int item = rand()%3; // random two item that agent place
    if(item == 0){
      printf("Agent places tobacco and match\n");
      V(&sp->paper);
    }
    else if(item == 1){
      printf("Agent places paper and tobacco\n");
      V(&sp->match);

    }
    else{
      printf("Agent places paper and match\n");
      V(&sp->tobacco);
    }
    sleep(500); // delap
    P(&sp->agent);
}
void smoker_remove(smoker_t *sp,char* msg){
    if(msg[0] == 'p'){
      P(&sp->tobacco);
      P(&sp->share);
      printf("Paper smoker smokes\n"); // critical section
      sleep(50); //delay
      V(&sp->share);
      V(&sp->agent);
    }
    if(msg[0] == 'm'){
      P(&sp->paper);
      P(&sp->share);
      printf("Match smoker smokes\n"); // critical section
      sleep(50); // delay
      V(&sp->share);
      V(&sp->agent);
    }
    if(msg[0] == 't'){
      P(&sp->match);
      P(&sp->share);
      printf("Tobacco smoker smokes\n"); // critical section 
      sleep(50); // delap
      V(&sp->share);
      V(&sp->agent);
    }
}