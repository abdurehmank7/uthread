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
    thread_t *t1;

    t1 = malloc(sizeof(thread_t));

    thread_create(main_t, NULL);

    thread_create(t1, &fun1);

    timer_start();

    thread_yield();

    thread_join(t1);
    LOG_OUT("Main thread exiting\n");

    timer_stop();

    return 0;
}
