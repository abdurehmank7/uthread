#include "thread.h"
#include "jmpbuf-offsets.h"
#include "util.h"
#include "time_control.h"

void Enqueue(Queue *queue, thread_t *thread)
{
    Node_t *node = malloc(sizeof(Node_t));
    node->thread = thread;
    node->next = NULL;
    node->prev = NULL;

    if (queue->head == NULL)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        node->prev = queue->tail;
        queue->tail = node;
    }

    queue->size++;
}

thread_t *Dequeue(Queue *queue)
{
    if (queue->head == NULL)
    {
        return NULL;
    }

    Node_t *node = queue->head;
    thread_t *thread = node->thread;

    if (queue->head == queue->tail)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        queue->head = node->next;
        queue->head->prev = NULL;
    }

    queue->size--;

    free(node);

    return thread;
}

void printQueue(Queue *queue)
{
    LOG_OUT("PRINTING QUEUE\n");
    Node_t *node = queue->head;
    while (node != NULL)
    {
        LOG_OUT("<Thread Id: %d, Thread State: %d> \n", node->thread->thread_id, node->thread->state);
        node = node->next;
    }
    LOG_OUT("PRINTING QUEUE END\n");
}

typedef struct
{
    int childID;
    thread_t *parentTCB;
} KeyValuePair;

typedef struct
{
    KeyValuePair array[MAX_MAP_SIZE];
    int size;
} Map;

// Function to initialize the map
void initializeMap(Map *map)
{
    map->size = 0;
    // printf("Map initialized\n");
}

// Function to add a key-value pair to the map
void insertMap(Map *map, int child_id, thread_t *thread)
{
    if (map->size < MAX_MAP_SIZE)
    {
        map->array[map->size].childID = child_id;
        map->array[map->size].parentTCB = thread;
        map->size++;
    }
    else
    {
        printf("Map is full.\n");
    }
}

// Function to remove a key-value pair from the map
thread_t *removeMap(Map *map, int child_id)
{
    int i;
    for (i = 0; i < map->size; i++)
    {
        if (map->array[i].childID == child_id)
        {
            thread_t *tcbToReturn = map->array[i].parentTCB;
            map->array[i].childID = -99;
            return tcbToReturn;
            break;
        }
    }
    return NULL;
}

// Function to print the map
void printMap(Map *map)
{
    LOG_OUT("PRINT MAP CALLED \n");
    int i;
    for (i = 0; i < map->size; i++)
    {
        printf("Key: %d, Value: %d\n", map->array[i].childID, map->array[i].parentTCB->thread_id);
    }
}

// Globals
extern struct itimerval timer;

thread_t *current_thread = NULL;

int number_of_threads;

Queue *ready_queue = NULL;
Queue *completed_queue = NULL;
Queue *waiting_queue = NULL;
Map blockedMap;

// functions
void init_lib()
{
    number_of_threads = 0;

    // Initializing Queues
    ready_queue = malloc(sizeof(Queue));
    ready_queue->head = NULL;
    ready_queue->tail = NULL;
    ready_queue->size = 0;

    completed_queue = malloc(sizeof(Queue));
    completed_queue->head = NULL;
    completed_queue->tail = NULL;
    completed_queue->size = 0;

    waiting_queue = malloc(sizeof(Queue));
    waiting_queue->head = NULL;
    waiting_queue->tail = NULL;
    waiting_queue->size = 0;

    initializeMap(&blockedMap);
}

int thread_create(thread_t *thread, void(*start_routine))
{

    if (number_of_threads == 0)
    {
        // LOG_OUT("Creating main thread\n");
        // We are creating main thread.
        thread->thread_id = number_of_threads++;
        thread->state = RUNNING;

        thread->stack = (char *)malloc(STACK_SIZE);
        thread->stack_size = STACK_SIZE;
        thread->stack_pointer = (addr_t)thread->stack + STACK_SIZE - sizeof(int);

        thread->callback_routine = NULL;

        thread->waiting_id = -1;
        thread->state = RUNNING;

        current_thread = thread;
    }
    else
    {
        // LOG_OUT("Creating thread %d\n", number_of_threads);
        thread->thread_id = number_of_threads++;

        thread->stack = (char *)malloc(STACK_SIZE);

        thread->stack_size = STACK_SIZE;
        thread->stack_pointer = (addr_t)thread->stack + STACK_SIZE - sizeof(int);

        thread->program_counter = (addr_t)start_routine;

        thread->waiting_id = -1;

        // Initialize the context

        int ret_val = sigsetjmp(thread->context, 1);
        (thread->context->__jmpbuf)[JB_RSP] = mangle(thread->stack_pointer);
        (thread->context->__jmpbuf)[JB_PC] = mangle(thread->program_counter);
        sigemptyset(&thread->context->__saved_mask);

        // Insert to Ready Queue
        thread->state = READY;
        Enqueue(ready_queue, thread);
        // printQueue(ready_queue);
    }

    return thread->thread_id;
}

void interrupt_generated(int signum)
{
    // printQueue(ready_queue);
    // write_formatted("Interrupt generated: %d\n", signum);

    disable_interrupts();

    // make scheduling decision here

    current_thread->state = READY;
    Enqueue(ready_queue, current_thread);

    if (ready_queue->size == 0)
    {
        LOG_OUT("No more threads to run\n");
        timer_reset();
        return;
    }

    thread_t *next_thread = Dequeue(ready_queue);
    next_thread->state = RUNNING;
    context_switch(current_thread, next_thread);
    return;
}

void context_switch(thread_t *prev, thread_t *next)
{
    // LOG_OUT("Context Switching from %d to %d\n", prev->thread_id, next->thread_id);
    current_thread = next;

    // reset the timer
    timer_reset(); // if problem occurs change the code to what abdullah said

    int ret_val = sigsetjmp(prev->context, 1);
    if (ret_val == 1)
    {
        enable_interrupts();
        return;
    }
    // The following is needed for the first call of the worker thread
    current_thread = next;
    current_thread->state = RUNNING;

    enable_interrupts();
    // // Call siglongjmp
    siglongjmp(current_thread->context, 1);
}

void thread_yield(void)
{
    // LOG_OUT("Hello from thread_yeild :: current->id:%d\n", current_thread->thread_id);
    disable_interrupts();

    current_thread->state = READY;
    Enqueue(ready_queue, current_thread);

    thread_t *next_thread = Dequeue(ready_queue);
    next_thread->state = RUNNING;
    context_switch(current_thread, next_thread);
}

void thread_exit()
{
    // LOG_OUT("Hello from thread_exit\n");
    // testing from here

    disable_interrupts();

    current_thread->state = COMPLETED;
    Enqueue(completed_queue, current_thread);

    // printMap(&blockedMap);

    // LOG_OUT("Current thread id: %d\n", current_thread->thread_id);
    thread_t *parent_thread = removeMap(&blockedMap, current_thread->thread_id);
    // LOG_OUT("Parent thread id: %d\n", parent_thread->thread_id);

    if (parent_thread != NULL)
    {
        // LOG_OUT("Parent thread found\n");
        parent_thread->state = READY;
        Enqueue(ready_queue, parent_thread);
    }

    // printf("waiting_id:%d \n", current_thread->waiting_id);

    thread_t *next_thread = Dequeue(ready_queue);
    next_thread->state = RUNNING;
    context_switch(current_thread, next_thread);
}

void thread_join(thread_t *thread)
{
    // LOG_OUT("Hello from thread_join\n");

    disable_interrupts();

    if (thread->state == COMPLETED)
    {
        // LOG_OUT("Thread %d already completed\n", thread->thread_id);
        return;
    }

    current_thread->state = BLOCKED;
    current_thread->waiting_id = thread->thread_id;
    insertMap(&blockedMap, thread->thread_id, current_thread);

    thread_t *next_thread = Dequeue(ready_queue);
    next_thread->state = RUNNING;
    context_switch(current_thread, next_thread);
}

void thread_sleep(unsigned int milliseconds)
{
    disable_interrupts();
    unsigned int start_time = get_time();
    unsigned int elapsed_time;

    do
    {
        // Calculate elapsed time
        elapsed_time = get_time() - start_time;

        // Sleep for a short interval to yield the CPU
        usleep(1000); // Sleep for 1 millisecond (adjust as needed)
    } while (elapsed_time < milliseconds);
    enable_interrupts();
}
