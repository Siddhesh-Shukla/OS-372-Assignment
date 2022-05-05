#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

#define SIZE (size_t)(1024*1024)

int main() 
{
    void *shared_memory;
    // char buff[100];
    int shmid = shmget((key_t)2346, SIZE, 0666);
    //                 //  identifier, size, read+write
    shared_memory = shmat(shmid, NULL, 0);
    // // printf("Address: %p\n", shared_memory);
    // // printf("Enter data:\n");
    // // read(0, buff, 100);
    // // strcpy(shared_memory, buff);
    printf("%d, %d\n", ((int*)shared_memory)[0], ((int*)shared_memory)[2]);
    // for(int i=0; i<2; i++)
    //     printf("Data: %s\n", (char*)(shared_memory+i*sizeof(char)*100));
}