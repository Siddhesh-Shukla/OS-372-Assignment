#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>   
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h> 
#include <pthread.h>

// #include "read-new-rr.c"
// #include "sum-new-rr.c"

struct RR_Args {
    int* glob_var;
    pthread_cond_t* cond;
    pthread_mutex_t* lock;
};

struct sumParams {
    int* array;
    long long* sum;
    int idx, size, leftIdx, rightIdx;
    void* shared_memory;
    // struct RR_Args2* args;
};

static int *glob_var;
pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#define SIZE (size_t)(1024*1024*1024)
#define MAX_THREADS 1024
int no_of_threads = 5;
int* isFinish;

struct fun_params {
    FILE* fp;
    int idx;
    long partitionLeftLimit;
    long partitionRightLimit;
    long size;
    int* storeReturnSize;
    int* store;
    // struct RR_Args1* args;
};


long getFileSize() {
    
    FILE* file = fopen("input.txt", "r"); 
    if (file == NULL) {
        return -1;
    }

    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;

}


FILE* moveTillNextNo(FILE* file, long* partition, int size) {
    
    fseek(file, *partition, SEEK_SET);
    
    while(1) {
        if(*partition == size) 
            break;
        char c; 
        fread(&c, sizeof(char), 1, file);
        if (c == ' ')
            break;
        fseek(file, ++(*partition), SEEK_SET);
    }

    return file;
}


void* getInput(void* args) {
    struct fun_params* params = (struct fun_params*) args;

    int value;
    
    while(params->partitionLeftLimit < params->partitionRightLimit) {
        
        pthread_mutex_lock(&lock);
        
        while (*glob_var == 1) {
            pthread_cond_wait(&cond, &lock);
        }

        params->partitionLeftLimit++;
        fscanf (params->fp, "%d", &value);
        params->store[params->storeReturnSize[params->idx]++] = value;
        
        params->fp = moveTillNextNo(params->fp, &params->partitionLeftLimit, params->size);

        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&cond);
    }

    isFinish[params->idx] = 1;

    printf("Input Thread: %d finished\n", params->idx);

    while (*glob_var == 0);
    pthread_cond_broadcast(&cond);

    return NULL;
}


void* mainRead(void* arg) 
{
    
    void *shared_memory;
    int shmid = shmget((key_t)2347, SIZE, 0666|IPC_CREAT);
    shared_memory = shmat(shmid, NULL, 0);

    void *shared_memory2;
    int shmid2 = shmget((key_t)3000, MAX_THREADS, 0666|IPC_CREAT);
    shared_memory2 = shmat(shmid2, NULL, 0);

    long size = getFileSize();
    if(size == -1) {
        printf("File Not Found!\n");
        return NULL;
    }
    printf("Size of File: %ld\n", size);
    // FILE* fout = fopen("time_taken.txt", "a");

    int* store[no_of_threads];
    
    int totalNo = 1000000;
    for(int i=0; i<no_of_threads; i++) {
        store[i] = (int*)malloc(totalNo*sizeof(int));
    }

    for(int i=0; i<no_of_threads; i++) {
        ((int*)shared_memory2)[i] = 0;
    }

    int value;
    int no_of_partitions = no_of_threads;
    long partition_idx[no_of_partitions+1];

    // open input file in all file pointers
    FILE** fps = (FILE**)malloc(no_of_partitions*sizeof(FILE*));
    for(int i=0; i<no_of_partitions; i++) {
        fps[i] = fopen("input.txt", "r");
    }

    // calculate partition index
    for(int i=0; i<=no_of_partitions; i++) {
        partition_idx[i] = (i*size)/no_of_partitions;
    }
    
    // move till next number in each file ptr
    for(int i=1; i<no_of_partitions; i++) {
        fps[i] = moveTillNextNo(fps[i], &partition_idx[i], size);
    }

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*no_of_threads);
    struct fun_params* params = (struct fun_params*)malloc(sizeof(struct fun_params)*no_of_threads);


    for(int i=0; i<no_of_partitions; i++) {
        params[i].fp = fps[i];
        params[i].idx = i;
        params[i].partitionLeftLimit = partition_idx[i];
        params[i].partitionRightLimit = partition_idx[i+1];
        params[i].size = size;
        params[i].store = store[i];
        params[i].storeReturnSize = (int*)shared_memory2;
        // params[i].args = (struct RR_Args1*)arg;
    }

    clock_t start, end;
    start = clock();

    for(int i=0; i<no_of_partitions; i++) {
        pthread_create(&threads[i], NULL, getInput, (void*)&params[i]);
    }

    for(int i=0; i<no_of_partitions; i++) {
        pthread_join(threads[i], NULL);
    }      
    
    end = clock();

    double time_taken = ((double)end - start) / ((double)CLOCKS_PER_SEC);

    // fprintf(fout,"%d %lf\n", no_of_threads, time_taken);

    FILE* fout1 = fopen("bt.txt", "r");
    double BT_ip, BT_sum;
    fscanf (fout1, "%lf %lf", &BT_ip, &BT_sum);
    printf("Burst Time to read : %lf\n", BT_ip);
    printf("TAT to read : %lf\n", time_taken);
    printf("Waiting Time to read : %lf\n", time_taken-BT_ip);

    for(int i=0; i<no_of_partitions; i++) {
        fclose(fps[i]);
    }
}


void* sum_array(void* arg)
{
    struct sumParams* params = (struct sumParams*) arg;
    int leftIdx = params->idx*1000000;
    int rightIndex = leftIdx + (((int*)(params->shared_memory))[params->idx]);
    // printf("%d %d\n", leftIdx, rightIndex);
    int i = params->leftIdx;

    // pthread_mutex_lock(lock);
    // printf("Hooray Sum\n");

    // while (*(params->args->glob_var) == 1) {
    //     pthread_cond_wait(params->args->cond, params->args->lock);
    //     printf("stucksum ");
    // }
    while(isFinish[params->idx] == 0) {

        pthread_mutex_lock(&lock);

        while (*glob_var == 0) {
            // printf("%d ", *glob_var);
            // printf("in wait\n");
            pthread_cond_wait(&cond, &lock);
        }
        
        rightIndex = leftIdx + (((int*)(params->shared_memory))[params->idx]);
        
        for (i; i < params->rightIdx; i++)
            params->sum[params->idx] += params->array[i];
        
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&cond);
    }

    rightIndex = leftIdx + (((int*)(params->shared_memory))[params->idx]);
    for (i; i < params->rightIdx; i++)
        params->sum[params->idx] += params->array[i];

    printf("Sum Thread: %d finished\n", params->idx);

    while (*glob_var == 1);
    pthread_cond_broadcast(&cond);
    return NULL;
}

void* mainSum(void* arg) 
{
    void *shared_memory;
    int shmid = shmget((key_t)2347, SIZE, 0666|IPC_CREAT);
    shared_memory = shmat(shmid, NULL, 0);

    void *shared_memory2;
    int shmid2 = shmget((key_t)3000, MAX_THREADS, 0666|IPC_CREAT);
    shared_memory2 = shmat(shmid2, NULL, 0);

    for(int i=0; i<no_of_threads; i++) {
        printf("%d ",((int*)shared_memory2)[i]);
    }
    printf("\n");
    
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
        args[i].shared_memory = shared_memory2;
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
    
    FILE* fout1 = fopen("bt.txt", "r");
    double BT_ip, BT_sum;
    fscanf (fout1, "%lf %lf", &BT_ip, &BT_sum);
    // printf(">> %lf %lf\n", BT_ip, BT_sum); 

    printf("Burst Time to sum : %lf\n", BT_sum);
    printf("TAT to sum : %lf\n", time_taken);
    printf("Waiting Time to sum : %lf\n", time_taken-BT_sum);
    
    printf("Sum is %lld\n",total_sum);

    // while (*glob_var == 1);
    // pthread_cond_broadcast(&cond);
}

int main()
{
    glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *glob_var = 0;

    isFinish = (int*)malloc(no_of_threads*sizeof(int));
    for(int i=0; i<no_of_threads; i++)
        isFinish[i] = 0;

    int id = fork();
    if(id == -1) {
        printf("Error while fork\n");
        return -1;
    }

    if(id == 0) {
        pthread_t th1, th2;
        struct RR_Args arg1 = {.glob_var=glob_var, .cond=&cond, .lock=&lock};
        struct RR_Args arg2 = {.glob_var=glob_var, .cond=&cond, .lock=&lock};

        pthread_create(&th1, NULL, mainRead, (void*) &arg1);
        pthread_create(&th2, NULL, mainSum, (void*) &arg2);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);
    }
    else {
        while(1) {
            usleep(1000);
            *glob_var = (*glob_var+1)%2;
            pthread_cond_broadcast(&cond);
        }
    }
}