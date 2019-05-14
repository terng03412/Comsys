/* $begin sbufc */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "common_threads.h"
#include "smoker.h"

// Set What's on the table at the initial state
/* $begin table_init */
void table_init(table_t *sp)
{
  Sem_init(&sp->can_place, 0, 1);     // Set Agent start placing item first 
  Sem_init(&sp->tobacco, 0, 0);       // Tobacco holder is waiting for items
  Sem_init(&sp->paper, 0, 0);         // Paper holder is waiting for items
  Sem_init(&sp->match, 0, 0);         // Match holder is waiting for items
}
/* $end table_init */

// Agent Place 2 items randomly 
/* $begin place_item */
void place_item(table_t *sp)
{
  P(&sp->can_place);                       // wait for all smoker smoked already

  // random 2 items -> there are all 3 possible pairing
  int item = rand()%3;

  // place Paper & Match
  if (item == 0){            
    printf("Agent places paper and match\n");
    V(&sp->tobacco);                      // Tobacco holder can smoke
  
  // place Tabacco & Match
  } else if (item == 1){
    printf("Agent places tobacco and match\n");              
    V(&sp->paper);                        // Paper holder can smoke

  // place Tabacco & Paper
  } else if (item == 2){
    printf("Agent places paper and tobacco\n");                         
    V(&sp->match);                        // Match holder can smoke
  }
}
/* $end place_item */

// Smoker is waiting for needed 2 items... then go smoke if get the items
/* $begin take_item */
void take_item(table_t *sp, int item)
{
  // Tobacco Holder
  if (item == 0){                    
    P(&sp->tobacco);                     // wait for Paper & Match
    printf("Tobacco smoker smokes\n");
    V(&sp->can_place);                   // Done smoke... Agent can place now

  // Paper Holder
  } else if (item == 1){                 
    P(&sp->paper);                      // wait for Tobacco & Match
    printf("Paper smoker smokes\n");
    V(&sp->can_place);                  // Done smoke... Agent can place now

  // Match Holder
  } else if (item == 2){                
    P(&sp->match);                      // wait for Tobacco and Paper
    printf("Match smoker smokes\n");
    V(&sp->can_place);                  // Done smoke... Agent can place now
  }
}
/* $end take_item */
