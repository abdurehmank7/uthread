#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

extern thread_t *current_thread;

int test_val = 0;
semaphore sem;

void fun1()
{
    LOG_OUT("Thread %d trying to acquire semaphore\n", current_thread->thread_id);
    sem_wait(&sem);
    LOG_OUT("Thread %d acquired semaphore\n", current_thread->thread_id);
    test_val++;
    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    if (current_thread->thread_id == 1 || current_thread->thread_id == 2)
    {
        thread_sleep(1000);
    }

    sem_post(&sem);
    LOG_OUT("Thread %d released semaphore\n", current_thread->thread_id);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));
    thread_t **t;
    t = malloc(sizeof(thread_t *) * 3);

    sem_init(&sem, 2);

    for (int i = 0; i < 3; i++)
    {
        t[i] = malloc(sizeof(thread_t));
    }

    thread_create(main_t, NULL);

    for (int i = 0; i < 3; i++)
    {
        thread_create(t[i], &fun1);
    }

    timer_start();

    thread_yield();

    for (int i = 0; i < 3; i++)
    {
        thread_join(t[i]);
    }

    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
