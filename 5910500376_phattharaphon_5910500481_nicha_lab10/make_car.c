#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0

int chassis = 0;
int seats = 0;
int tires = 0;
int cover = 0;
int engine = 0;
int readyToPaint = 0;
int count = 1;

pthread_mutex_t lock;
pthread_cond_t reqChassis;
pthread_cond_t reqAssembly;
pthread_cond_t reqCover;
pthread_cond_t reqPaint;

void addChassis()
{
    chassis += 1;
    printf("\nCar #%d\n", count++);
    // printf("Added Chassis\n");
    printf("B puts chassis on the conveyor\n");
}

void addSeat()
{
    seats += 1;
    // printf("Added Seat\n");
    printf("C attaches seats\n");
}

void addEngine()
{
    engine += 1;
    // printf("Added Engine\n");
    printf("D places engine\n");
}

void addTires()
{
    tires += 1;
    // printf("Added tires\n");
    printf("A puts tires\n");
}

void addCover()
{
    cover += 1;
    // printf("Added cover\n");
    printf("D assembles top cover\n");
    readyToPaint = TRUE;
}

void Paint()
{
    chassis = 0;
    seats = 0;
    tires = 0;
    cover = 0;
    engine = 0;
    // printf("Paint \n");
    printf("A paints\n");
    readyToPaint = FALSE;
}

int readyToCover()
{
    int ans = TRUE;
    ans = ans && tires;
    ans = ans && engine;
    ans = ans && seats;
    return ans;
}

void *CoverAdder(void *arg)
{
    pthread_mutex_lock(&lock);
    while (1)
    {
        while (cover >= 1 || !readyToCover())
        {
            pthread_cond_wait(&reqCover, &lock);
        }
        addCover();
        sleep(1);
        pthread_cond_signal(&reqPaint);
    }
}

void *Paintt(void *arg)
{
    pthread_mutex_lock(&lock);
    while (1)
    {

        while (!readyToPaint)
        {
            pthread_cond_wait(&reqPaint, &lock);
        }
        Paint();
        sleep(1);
        pthread_cond_signal(&reqChassis);
    }
}
void *worker_d(void *arg)
{
    pthread_mutex_lock(&lock);
    while (1)
    {
        while (engine >= 1)
        {
            pthread_cond_wait(&reqAssembly, &lock);
        }
        addEngine();
        sleep(1);
        pthread_cond_signal(&reqCover);

        while (cover >= 1 || !readyToCover())
        {
            pthread_cond_wait(&reqCover, &lock);
        }
        addCover();
        sleep(1);
        pthread_cond_signal(&reqPaint);
    }
}

void *worker_c(void *arg)
{
    pthread_mutex_lock(&lock);
    while (1)
    {
        while (seats >= 1 || chassis == 0)
        {
            pthread_cond_wait(&reqAssembly, &lock);
        }
        addSeat();
        sleep(1);
        pthread_cond_signal(&reqCover);
    }
}

void *worker_b(void *arg)
{
    pthread_mutex_lock(&lock);
    while (1)
    {
        while (chassis >= 1 || readyToPaint)
        {
            pthread_cond_wait(&reqChassis, &lock);
        }
        addChassis();
        sleep(1);
        pthread_cond_broadcast(&reqAssembly);
    }
}

void *worker_a(void *arg)
{
    pthread_mutex_lock(&lock);
    while (1)
    {
        while (tires >= 1 || chassis == 0)
        {
            pthread_cond_wait(&reqAssembly, &lock);
        }
        addTires();
        sleep(1);
        pthread_cond_signal(&reqCover);

        while (!readyToPaint)
        {
            pthread_cond_wait(&reqPaint, &lock);
        }
        Paint();
        sleep(1);
        pthread_cond_signal(&reqChassis);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&lock, 0);
    pthread_cond_init(&reqChassis, 0);
    pthread_cond_init(&reqAssembly, 0);
    pthread_cond_init(&reqCover, 0);
    pthread_cond_init(&reqPaint, 0);

    pthread_t thread1, thread2, thread3, thread4;

    pthread_create(&thread1, NULL, worker_a, NULL);
    pthread_create(&thread2, NULL, worker_b, NULL);
    pthread_create(&thread3, NULL, worker_c, NULL);
    pthread_create(&thread4, NULL, worker_d, NULL);

    while (1)
    {
        sleep(1000);
    }
    return 0;
}
