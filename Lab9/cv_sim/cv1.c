#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

// void thr_exit()
// {
//     // sleep(1);
//     printf("e0\n");
//     Pthread_mutex_lock(&m);
//     printf("e1\n");
//     done = 1;
//     Pthread_cond_signal(&c);
//     printf("e2\n");
//     Pthread_mutex_unlock(&m);
//     printf("e3\n");
// }

// void thr_join()
// {

//     printf("j0\n");
//     Pthread_mutex_lock(&m);
//     printf("j1\n");
//     while (done == 0)
//         Pthread_cond_wait(&c, &m);
//     printf("j2\n");
//     Pthread_mutex_unlock(&m);
//     printf("j3\n");
// }
void thr_exit()
{
    printf("e0\n");
    Pthread_mutex_lock(&m);
    printf("e1\n");
    Pthread_cond_signal(&c);
    printf("e2\n");
    Pthread_mutex_unlock(&m);
    printf("e3\n");
}
void thr_join()
{
    printf("j0\n");
    Pthread_mutex_lock(&m);
    printf("j1\n");
    Pthread_cond_wait(&c, &m);
    printf("j2\n");
    Pthread_mutex_unlock(&m);
    printf("j3\n");
}

void *child(void *arg)
{
    printf("child\n");
    thr_exit();
    return NULL;
}
int main(int argc, char *argv[])
{
    printf("parent: begin\n");
    pthread_t p;
    Pthread_create(&p, NULL, child, NULL);
    thr_join();
    printf("parent: end\n");
    return 0;
}
