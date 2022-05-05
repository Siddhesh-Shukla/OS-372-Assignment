#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


struct fun_params {
    FILE* fp;
    int idx;
    long partitionLeftLimit;
    long partitionRightLimit;
    long size;
    int* storeReturnSize;
    int* store;
};


long getFileSize()
{
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
        
        params->partitionLeftLimit++;
        fscanf (params->fp, "%d", &value);
        params->store[params->storeReturnSize[params->idx]++] = value;
        // printf("%d\n", value);
        
        params->fp = moveTillNextNo(params->fp, &params->partitionLeftLimit, params->size);
    }

    return NULL;
}

#define SIZE (size_t)(1024*1024*1024)
#define MAX_THREADS 1024

int main()
{

    void *shared_memory;
    int shmid = shmget((key_t)2347, SIZE, 0666|IPC_CREAT);
    shared_memory = shmat(shmid, NULL, 0);

    int shmid2 = shmget((key_t)3000, MAX_THREADS, 0666|IPC_CREAT);
    void *shared_memory2 = shmat(shmid2, NULL, 0);

    int no_of_threads=5;
    int totalNo = 1000000;

    long size = getFileSize();
    if(size == -1) {
        printf("File Not Found!\n");
        return -1;
    }
    

    printf("Size of File: %ld\n", size);
    FILE* fout = fopen("time_taken.txt", "a");


    int* store[no_of_threads];
    for(int i=0; i<no_of_threads; i++) {
        store[i] = (int*)malloc(totalNo*sizeof(int));
    }

    for(int i=0; i<no_of_threads; i++) {
        ((int*)shared_memory2)[i] = 0;
    }

    int value;
    int no_of_partitions = no_of_threads;
    long partition_idx[no_of_partitions+1];

    FILE** fps = (FILE**)malloc(no_of_partitions*sizeof(FILE*));
        
    // open input file in all file pointers
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


    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*no_of_partitions);
    struct fun_params* params = (struct fun_params*)malloc(sizeof(struct fun_params)*no_of_partitions);


    for(int i=0; i<no_of_partitions; i++) {
        params[i].fp = fps[i];
        params[i].idx = i;
        params[i].partitionLeftLimit = partition_idx[i];
        params[i].partitionRightLimit = partition_idx[i+1];
        params[i].size = size;
        params[i].store = store[i];
        params[i].storeReturnSize = (int*)shared_memory2;
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

    for(int i=0; i<no_of_partitions; i++) {
        fclose(fps[i]);
    }

    // int storeReturnSize[no_of_threads];

    // for(int i=0; i<no_of_threads; i++) {
    //     storeReturnSize[i] = params[i].storeReturnSize;
    // }


    double time_taken = ((double)end - start) / ((double)CLOCKS_PER_SEC);
    printf("Time taken by to take input : %lf\n", time_taken);

    fprintf(fout,"%d %lf\n", no_of_threads, time_taken);

    free(fps);
    free(threads);
    free(params);

    for(int i=0; i<no_of_partitions; i++) {
        for(int j=0; j<((int*)shared_memory2)[i]; j++) {
            ((int*)shared_memory)[i*1000000+j] = store[i][j];
        }
    }

    // for(int i=0; i<no_of_threads; i++) {
    //     ((int*)shared_memory2)[i] = storeReturnSize[i];
    // }

    return 0;
}