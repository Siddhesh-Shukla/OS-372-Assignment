#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

static int *glob_var;

pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// struct RR_Args {
//     pthread_cond_t* cond;
//     pthread_mutex_t* lock;
// };

void print() {
    printf(">> %d\n", *glob_var);
}

void* yes(void* args) {
    // struct RR_Args* params = (struct RR_Args*) args;

    while(1) {
        
        pthread_mutex_lock(&lock);
        
        while (*glob_var == 1) {
            pthread_cond_wait(&cond, &lock);
        }
        while (*glob_var == 0) {
            printf("Yes ");
        }
        
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&cond);
        
    }
}

void* no(void* args) {
    // struct RR_Args* params = (struct RR_Args*) args;

    while(1) {
        
        pthread_mutex_lock(&lock);
        
        while (*glob_var == 0) {
            pthread_cond_wait(&cond, &lock);
        }
        while (*glob_var == 1) {
            printf("No ");
        }
        
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&cond);
        
    }
}

int main() {
    glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *glob_var = 0;

    int id = fork();
    if(id == -1) {
        printf("Error while fork\n");
        return -1;
    }
    if(id == 0) {
        pthread_t th1, th2;

        // struct RR_Args arg1 = {.cond=&cond, .lock=&lock};
        // struct RR_Args arg2 = {.cond=&cond, .lock=&lock};

        pthread_create(&th1, NULL, yes, NULL);
        pthread_create(&th2, NULL, no, NULL);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);
    }
    else {
        while(1) {
            usleep(100);
            *glob_var = (*glob_var+1)%2;
            // pthread_cond_signal(&cond);
        }
    }

}