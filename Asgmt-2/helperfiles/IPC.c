# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

int main(int argc, char* argv[]) {
    int fd[2];
    // fd[0] - read
    // fd[1] - write
    if(pipe(fd) == -1) {
        printf("Error Occured while entering pipe\n");
        return 1;
    }
    
    int id = fork();
    if(id == -1) {
        printf("Error Occured in fork\n");
        return 2;
    }    
    if(id == 0) {
        // child process
        close(fd[0]);
        int x;
        printf("Input a Number: ");
        scanf("%d", &x);
        if(write(fd[1], &x, sizeof(int)) == -1) {
            printf("Error occured while writing\n");
            return 3;
        }
        int z = 23;
        if(write(fd[1], &z, sizeof(int)) == -1) {
            printf("Error occured while writing\n");
            return 3;
        }
        close(fd[1]);
    }
    else {
        // parent process
        close(fd[1]);
        int y;
        if(read(fd[0], &y, sizeof(int)) == -1) {
            printf("Error occured while reading\n");
            return 4;
        }
        printf("From Child Process: %d\n", y);
        if(read(fd[0], &y, sizeof(int)) == -1) {
            printf("Error occured while reading\n");
            return 4;
        }
        printf("From Child Process: %d\n", y);
        close(fd[0]);
    }

    return 0;
}