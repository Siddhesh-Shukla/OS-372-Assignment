#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main() {
    int id = fork();
    if(id == 0) {
        for(int i=0; i<1000; i++)
            printf("yaay ");
    }
    else {
        wait(NULL);
        for(int i=0; i<1000; i++)
            printf("nooo ");
    }
}