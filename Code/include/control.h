
#ifndef CONTROL_H
#define CONTROL_H


// #define MAX_THREADS 64  // maximum number of threads

typedef struct mutex_t
{
    int flag;
} mutex;

typedef struct semaphore_t
{
    int value;

} semaphore;




void mutex_init(mutex *m);
void mutex_acquire(mutex *m);
void mutex_release(mutex *m);

void sem_init(semaphore *sem, int value);
void sem_wait(semaphore *sem);
void sem_post(semaphore *sem);

#endif // CONTROL_H