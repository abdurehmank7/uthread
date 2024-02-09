#include "control.h"
#include "thread.h"
#include "util.h"

#include <stdio.h>

extern thread_t *current_thread;
extern Queue *waiting_queue;
extern Queue *ready_queue;

// Test and Set function
int LOW_KEY(volatile int *addr, int newval)
{
    int result = newval;
    asm volatile("lock; xchg %0, %1"
                 : "+m"(*addr), "=r"(result)
                 : "1"(newval)
                 : "cc");
    return result;
}

void mutex_init(mutex *m)
{
    m->flag = 0;
}
void mutex_acquire(mutex *m)
{
    while (LOW_KEY(&m->flag, 1) == 1)
    {
        ;
    }
}
void mutex_release(mutex *m)
{
    m->flag = 0;
}

// ________________________________________________

void sem_init(semaphore *sem, int value)
{
    sem->value = value;
}
void sem_wait(semaphore *sem)
{
    disable_interrupts();
    sem->value--;
    if (sem->value < 0)
    {
        // Block the thread
        current_thread->state = BLOCKED;
        // Add the thread to waiting queue
        Enqueue(waiting_queue, current_thread);
        // Call scheduler
        // current_thread->state = READY;
        // Enqueue(ready_queue, current_thread);

        if (ready_queue->size == 0)
        {
            LOG_OUT("No more threads to run\n");
            // timer_reset();
            return;
        }

        thread_t *next_thread = Dequeue(ready_queue);
        next_thread->state = RUNNING;
        context_switch(current_thread, next_thread);
    }
    enable_interrupts();
}
void sem_post(semaphore *sem)
{
    disable_interrupts();

    sem->value++;
    if (sem->value <= 0)
    {
        // Unblock the thread
        thread_t *thread = Dequeue(waiting_queue);
        thread->state = READY;
        // Add the thread to ready queue
        Enqueue(ready_queue, thread);
    }
    enable_interrupts();
}
