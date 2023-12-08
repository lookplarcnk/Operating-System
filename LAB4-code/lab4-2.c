#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int x = 0;
sem_t m;

void *thread(void *arg)
{
        sem_wait(&m);
        x = x + 1;
        sem_post(&m);
}
void main()
{
        pthread_t tid[10];
        int i;

        if (sem_init(&m, 0, 1) == -1) {
        perror("Could not initialize mylock semaphore");
        exit(2);
        }
        for (i=0; i<10; i++)
{
        if (pthread_create(&tid[i], NULL, thread, NULL) < 0) {
                perror("Error: thread cannot be created");
                exit(1);
        }
}
        for (i=0; i<10; i++) pthread_join(tid[i], NULL);
        printf("Final value of x is %d\n", x);
        exit(0);
}