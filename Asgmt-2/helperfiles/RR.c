#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


pthread_cond_t condchild[3];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void* child1(void* arg){

pthread_mutex_lock(&lock);
pthread_cond_wait(&condchild[0], &lock);
// paste child 1 code here




pthread_cond_signal(&condchild[1]);

// if(i == n1) then release lock and move on to the next
pthread_mutex_unlock(&lock);


}
void* child2(void* arg){

pthread_mutex_lock(&lock);
pthread_cond_wait(&condchild[1], &lock);
// paste child 2 code here



pthread_cond_signal(&condchild[2]);
pthread_mutex_unlock(&lock);


}
void* child3(void* arg){

pthread_mutex_lock(&lock);

pthread_cond_wait(&condchild[2], &lock);
// paste child 3 code here


pthread_mutex_unlock(&lock);


}



int main(int argc, char* argv[]) {
    pthread_t th[3];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&condchild, NULL);
    for (int i = 0; i < 3; i++) {
        if (i == 1) {
            if(pthread_create(&th[i], NULL, &child2, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
        else if(i == 0){
                        if(pthread_create(&th[i], NULL, &child2, NULL) != 0) {
                        perror("Failed to create thread");
                } 
            
             }
             else if(i == 2){
                        if(pthread_create(&th[i], NULL, &child3, NULL) != 0) {
                        perror("Failed to create thread");
                } 
            
             }
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&condchild);


    return 0;
}