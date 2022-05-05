#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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

int main()
{
    int array[] = { 1, 5, 7, 10, 12, 14, 15, 18, 20, 22, 25, 27, 30, 64, 110, 220, 101, 1000, 234 };
    int size = sizeof(array)/sizeof(array[0]);
    int no_of_threads = 5;
    pthread_t threads[no_of_threads];
    struct sumParams* args = (struct sumParams*)malloc(no_of_threads*sizeof(struct sumParams));
    
    int sum[no_of_threads];
    for(int i=0; i<no_of_threads; i++)
        sum[i] = 0;
    
    for (int i = 0; i < no_of_threads; i++) {
        args[i].idx = i;
        args[i].sum = sum;
        args[i].array = array;
        args[i].size = size;
        args[i].leftIdx = (i*size)/no_of_threads;
        args[i].rightIdx = ((i+1)*size)/no_of_threads;
    }

    for (int i = 0; i < no_of_threads; i++) {
        pthread_create(&threads[i], NULL, sum_array, (void*)&args[i]);
    }
    
    for (int i = 0; i < no_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    int total_sum = 0;
    for (int i = 0; i < no_of_threads; i++)
        total_sum += sum[i];
    
    printf("Sum is %d\n",total_sum);
    
    return 0;
}