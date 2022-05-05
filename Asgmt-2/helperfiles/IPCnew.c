// shmget() create share memory
// shmat() to attach shared segment with address space of process

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

// int shmget(key_t key, size_t size, int shmflg);
// void *shmat(int shmid, const void* shmadddr, int shmflg);

