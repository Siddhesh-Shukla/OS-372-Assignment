#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


struct fun_params {
    FILE* fp;
    long partitionLeftLimit;
    long partitionRightLimit;
    long size;
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
        printf("%d\n", value);
        
        params->fp = moveTillNextNo(params->fp, &params->partitionLeftLimit, params->size);
    }

    return NULL;
}


struct sumParams {
    int* array;
    int* sum;
    int idx, size, leftIdx, rightIdx;
};

    
void* sum_array(void* arg)
{
    struct sumParams* params = (struct sumParams*) arg;
    
    for (int i = params->leftIdx; i < params->rightIdx; i++)
        params->sum[params->idx] += params->array[i];

    return NULL;
}

