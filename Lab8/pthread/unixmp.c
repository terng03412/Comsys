/* This example is copied from */
/* <http://www.coe.uncc.edu/~abw/parallel/pthreads/pthreads.html */
/* It illustrates the idea of shared memory programming */
/* aug-16-2000 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void first_function(int *);
void second_function(int *);
void final_function(int,int);

int shared_mem_id;
int *shared_mem_ptr;
int *i, *j;

main()
{
        pid_t child1_pid, child2_pid;
        int state;
        /* initialise the shared memmory segment */
        shared_mem_id = shmget(IPC_PRIVATE, 2*sizeof(int),(0600 | IPC_CREAT));
        shared_mem_ptr = (int *)shmat(shared_mem_id, (void*)0, 0);
        
        /* giving the pointers to the variables */
        i = shared_mem_ptr;
        j = (shared_mem_ptr + 1);
        *i = 0;
        *j =0;
        
        /* start the first child process */
        if((child1_pid = fork())== 0)
        {
                /* job of first child*/
                first_function(i);
                exit();
        }
        
        /*start the second child process*/
        if((child2_pid = fork())== 0)
        {
                /* job of second child*/
                second_function(j);
                exit();
        }
        
        /*parent*/
        waitpid(child1_pid, state,0);
        waitpid(child2_pid, state,0);
        
        final_function(*i, *j);
        
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
