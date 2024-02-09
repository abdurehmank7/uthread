#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

extern thread_t *current_thread;

int test_val = 0;
mutex m;

void fun1()
{
    LOG_OUT("Thread %d trying to acquire mutex\n", current_thread->thread_id);
    mutex_acquire(&m);
    LOG_OUT("Thread %d acquired mutex\n", current_thread->thread_id);
    test_val++;
    if (current_thread->thread_id == 1 || current_thread->thread_id == 2)
    {
        thread_yield();
    }
    LOG_OUT("Thread %d releasing mutex\n", current_thread->thread_id);
    mutex_release(&m);

    LOG_OUT("Value of test_val: %d in thread %d\n", test_val, current_thread->thread_id);
    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));
    thread_t *t1, *t2, *t3;

    mutex_init(&m);

    t1 = malloc(sizeof(thread_t));
    t2 = malloc(sizeof(thread_t));
    t3 = malloc(sizeof(thread_t));

    thread_create(main_t, NULL);

    thread_create(t1, &fun1);
    thread_create(t2, &fun1);
    thread_create(t3, &fun1);

    timer_start();

    thread_yield();

    thread_join(t1);
    thread_join(t2);
    thread_join(t3);

    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
