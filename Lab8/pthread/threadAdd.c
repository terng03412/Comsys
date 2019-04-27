#include <stdio.h>
#include <pthread.h>
#define array_size 1000
#define no_threads 10
/* shared data */
int a[array_size]; /* array of numbers to sum */
int global_index = 0; /* global index */
int sum = 0; /* final result, also used by slaves */ 
pthread_mutex_t mutexl; /* mutually exclusive lock variable */ 
void *slave(void *ignored) /* Slave threads */
{
    int local_index, partial_sum = 0;
    do 
    {
	pthread_mutex_lock(&mutexl);/* get next index into the array */ 
	local_index = global_index; /* read current index & save locally*/ 
	global_index++;             /* increment global index */
	pthread_mutex_unlock(&mutexl);

	if (local_index < array_size) partial_sum += *(a + local_index);
    } while (local_index < array_size) ;

    pthread_mutex_lock(&mutexl); /* add partial sum to global sum */
    sum += partial_sum;
    pthread_mutex_unlock(&mutexl);
    return (0);                  /* Thread exits */
} /* slave */

int main ( ) 
{
    int i;
    pthread_t thread[10];             /* threads */ 
    pthread_mutex_init(&mutexl,NULL); /* initialize mutex */
    for (i = 0; i < array_size; i++) /* initialize a[] */
	a[i] = i+1;
    for (i = 0; i < no_threads; i++) /* create threads */ 
	if (pthread_create(&thread[i], NULL, slave, NULL) != 0)
	    perror("Pthread_create fails");
    for (i = 0; i < no_threads; i++) /* join threads */
	if (pthread_join(thread[i], NULL) != 0)
	    perror("Pthread_join fails");
    printf("The sum of 1 to %i is %d\n", array_size, sum) ;
    return (0);
}/* end of main */

