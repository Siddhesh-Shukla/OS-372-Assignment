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
    // char buff[2][20];
    int shmid = shmget((key_t)2346, SIZE, 0666|IPC_CREAT);
                    //  identifier, size, read+write
    // printf("%d\n", shmid);
    shared_memory = shmat(shmid, NULL, 0);
    // printf("Address: %p\n", shared_memory);
    int m, n;
    
    printf("Enter data: \n");
    scanf("%d", &n);
    ((int*)shared_memory)[0] = n;

    printf("Enter data: \n");
    scanf("%d", &m);
    ((int*)shared_memory)[2] = m;

    // read(0, buff[0], 100);
    // printf("%s\n", buff[0]);
    // printf("Enter data: \n");
    // read(0, buff[1], 100);
    // printf("%s\n", buff[1]);

    // printf("%d, %d\n", ((int*)shared_memory)[0], ((int*)shared_memory)[2]);

    // for(int i=0; i<2; i++)
    //     strcpy(shared_memory+100*i, buff[1]);


    // printf("%p %p\n", shared_memory, (shared_memory)+1*100);

    // printf("Given i/p: %s", (char*)shared_memory);
    // printf("Given i/p: %s", (char*)(shared_memory)+1*100);
}