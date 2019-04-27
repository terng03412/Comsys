/* This example is copied from */
/* <http://www.coe.uncc.edu/~abw/parallel/pthreads/pthreads.html */
/* It illustrates the idea of shared memory programming */
/* aug-16-2000 */

#include <pthread.h>

void first_function(int *);
void second_function(int *);
void final_function(int,int);

int i=0, j=0;

main()
{
        pthread_t thread1, thread2;
        pthread_create(&thread1, NULL, (void*) first_function, (void*)&i);
        pthread_create(&thread2, NULL, (void*) second_function, (void*)&j);
        
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
         
        final_function(i,j);
}

void first_function(int *num)
{
        int x,y,z;
        for(x=0; x< 10; x++)
        {
                printf("Inside the first_function\n");
                for(y=0; y< 1000; y++)
                z =z+1; /* introduce delay*/
                (*num)++;
        }
}

void second_function(int *num)
{
        int x,y,z;
        for(x=0; x< 10; x++)
        {
                printf("Inside the second_function\n");
                for(y=0; y< 1000; y++)
                z =z+1; /* introduce delay*/
                (*num)++;
        }
}

void final_function(int first, int second)
{
        int final_score;
        final_score = first + second ;
        printf("In final : first = %d, second = %d and final_score = %d\n",
        first, second, final_score);
}
