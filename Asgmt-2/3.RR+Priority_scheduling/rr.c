#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;
int fuel = 0;

void* fuel_filling(void* args) {
    for(int i=0; i<5; i++) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 60;
        printf("After Filling: %d\n", fuel);

        pthread_mutex_unlock(&mutexFuel);
        // pthread_cond_signal(&condFuel);
        pthread_cond_broadcast(&condFuel);
        sleep(1);
    }
}

void* car(void* args) {
    pthread_mutex_lock(&mutexFuel);

    while(fuel<40) {
        printf("Insufficient Fuel\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
        // equivalent to unlock, wait for signal, lock
    }

    fuel -= 40;
    printf("Fuel Received, Fuel Left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main() {
    pthread_t th[5];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);

    for(int i=0; i<5; i++) {
        if(i == 4)
            pthread_create(&th[i], NULL, fuel_filling, NULL);
        else
            pthread_create(&th[i], NULL, car, NULL);
    }

    for(int i=0; i<5; i++) {
        pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
}