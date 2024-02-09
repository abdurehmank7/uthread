#include "thread.h"
#include "control.h"
#include "time_control.h"
#include "util.h"

extern thread_t *current_thread;

void fun1()
{
    for (int i = 0; i < 5; i++)
    {
        LOG_OUT("Thread %d: %d\n", current_thread->thread_id, i);
        thread_yield();
    }

    thread_exit();
}

int main()
{
    init_lib();
    thread_t *main_t = malloc(sizeof(thread_t));
    thread_t *t1, *t2, *t3, *t4, *t5, *t6, *t7, *t8;

    t1 = malloc(sizeof(thread_t));
    t2 = malloc(sizeof(thread_t));
    t3 = malloc(sizeof(thread_t));
    t4 = malloc(sizeof(thread_t));
    t5 = malloc(sizeof(thread_t));
    t6 = malloc(sizeof(thread_t));
    t7 = malloc(sizeof(thread_t));
    t8 = malloc(sizeof(thread_t));

    thread_create(main_t, NULL);

    thread_create(t1, &fun1);
    thread_create(t2, &fun1);
    thread_create(t3, &fun1);
    thread_create(t4, &fun1);
    thread_create(t5, &fun1);
    thread_create(t6, &fun1);
    thread_create(t7, &fun1);
    thread_create(t8, &fun1);

    timer_start();

    thread_yield();

    thread_join(t8);
    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
