#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int x = 0;
sem_t m;

void *thread(void *arg) {
        sem_wait(&m);
        x++;
        printf("Thread ID: %1u, x = %d\n", pthread_self(), x);
        sem_post(&m);
        return NULL;
}

int main() {
        pthread_t tid[10];
        int i;

        if (sem_init(&m, 0, 1) == -1) {
                perror("Could not initialize semaphore");
                exit(2);
        }

        for (i = 0; i < 10; i++) {
                if (pthread_create(&tid[i], NULL, thread, NULL) < 0) {
                perror("Error creating thread");
                        exit(1);
                }
        }

        for (i = 0; i < 10; i++) {
                pthread_join(tid[i], NULL);
        }

        printf("Final value of x is %d\n", x);
        sem_destroy(&m);
        exit(0);

}