#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h> 
#include <pthread.h>
// #include "rr.c"


struct RR_Args2 {
    int* glob_var;
    pthread_cond_t* cond;
    pthread_mutex_t* lock;
};


struct sumParams {
    int* array;
    long long* sum;
    int idx, size, leftIdx, rightIdx;
    struct RR_Args2 * args;
};


void* sum_array(void* arg)
{
    struct sumParams* params = (struct sumParams*) arg;
    int i = params->leftIdx;

    pthread_mutex_lock(params->args->lock);
    printf("Hooray Sum\n");

    while (*(params->args->glob_var) == 1) {
        pthread_cond_wait(params->args->cond, params->args->lock);
        printf("stucksum ");
    }
    
    for (i; i < params->rightIdx; i++)
        params->sum[params->idx] += params->array[i];

    pthread_mutex_unlock(params->args->lock);
    pthread_cond_broadcast(params->args->cond);

    return NULL;
}

#define SIZE (size_t)(1024*1024*1024)
#define MAX_THREADS 1024

void* mainSum(void* arg) 
{

    // printf("sum-fn\n");
    void *shared_memory;
    int shmid = shmget((key_t)2347, SIZE, 0666|IPC_CREAT);
    shared_memory = shmat(shmid, NULL, 0);

    int shmid2 = shmget((key_t)3000, MAX_THREADS, 0666|IPC_CREAT);
    void *shared_memory2 = shmat(shmid2, NULL, 0);

    int no_of_threads = 5;

    // for(int i=0; i<no_of_threads; i++) {
    //     printf("%d ",((int*)shared_memory2)[i]);
    // }
    // printf("\n");

    while (*(((struct RR_Args2*)arg)->glob_var) == 0) {
        pthread_cond_wait(((struct RR_Args2*)arg)->cond, ((struct RR_Args2*)arg)->lock);
    }
    
    // for(int i=0; i<no_of_threads; i++) {
    //     for(int j=0; j<((int*)shared_memory2)[i]; j++) {
    //         printf("%d ", ((int*)shared_memory)[i*1000000+j]);
    //     }
    // }

    pthread_t threads[no_of_threads];
    struct sumParams* args = (struct sumParams*)malloc(no_of_threads*sizeof(struct sumParams));
    
    long long sum[no_of_threads];
    for(int i=0; i<no_of_threads; i++) {
        sum[i] = 0;   
    }

    for (int i = 0; i < no_of_threads; i++) {
        args[i].idx = i;
        args[i].sum = sum;
        args[i].array = shared_memory;
        args[i].size = -1;
        args[i].leftIdx = i*1000000;
        args[i].rightIdx = i*1000000+((int*)shared_memory2)[i];
    }

    clock_t start, end;
    start = clock();

    for (int i = 0; i < no_of_threads; i++) {
        pthread_create(&threads[i], NULL, sum_array, (void*)&args[i]);
    }
    
    for (int i = 0; i < no_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    long long total_sum = 0;
    for (int i = 0; i < no_of_threads; i++)
        total_sum += sum[i];

    end = clock();

    double time_taken = ((double)end - start) / ((double)CLOCKS_PER_SEC);
    printf("Time taken by to sum : %lf\n", time_taken);
    
    printf("Sum is %lld\n",total_sum);

    // printf("%d, %d\n", ((int*)shared_memory)[0], ((int*)shared_memory)[2]);
    // for(int i=0; i<2; i++)
    //     printf("Data: %s\n", (char*)(shared_memory+i*sizeof(char)*100));
    return NULL;
}