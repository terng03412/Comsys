#ifndef __SBUF_H__
#define __SBUF_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* $begin table */
typedef struct {
  sem_t can_place;              // Semaphore to signal Agent to place item or not
  sem_t tobacco;                // Semaphore to signal Tobacco smoker can smoke with on table items or not
  sem_t paper;                  // Semaphore to signal Paper smoker can smoke with on table items or not
  sem_t match;                  // Semaphore to signal Match smoker can smoke with on table itmes or not
} table_t;
/* $end table */

void table_init(table_t *sp);           // function to create initial state (Agent place item first)
void place_item(table_t *sp);           // function of Agent operating
void take_item(table_t *sp, int item);  // function for all smoker to operate

#endif /* __TABLE_H__ */
