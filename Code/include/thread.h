#ifndef THREAD_H
#define THREAD_H

#include <signal.h>
#include <setjmp.h>
#include "util.h"

#define STACK_SIZE 4096 // in bytes
#define MAX_THREADS 64  // maximum number of threads

#define MAX_MAP_SIZE 1000

enum THREAD_STATES
{
    RUNNING,
    READY,
    BLOCKED,
    COMPLETED
};

typedef struct thread
{
    // Key elements to be tracked within the TCB include the thread ID, the thread's current state, the allocated stack for the thread, the program counter (initially set to the address of the designated function to be executed), and the stack pointer.
    int thread_id;
    enum THREAD_STATES state;

    // stack
    char *stack;
    int stack_size;

    addr_t stack_pointer;
    addr_t program_counter;

    // Context switching
    jmp_buf context;

    // Callback function
    void (*callback_routine)(void);
    
    // Waiting id of the thread that is waiting on this thread, incase a thread calls join on this thread
    int waiting_id;

} thread_t; // struct for defining thread control block

void init_lib();
int thread_create(thread_t *thread, void(*start_routine));
void interrupt_generated(int signum);
void context_switch(thread_t *prev, thread_t *next);
void thread_exit();
void thread_join(thread_t *thread);
void thread_sleep(unsigned int milliseconds);
void thread_yield(void);

typedef struct Node_t
{
    thread_t *thread;
    struct Node_t *next;
    struct Node_t *prev;
} Node_t;

typedef struct
{
    Node_t *head;
    Node_t *tail;
    int size;
} Queue;

void Enqueue(Queue *queue, thread_t *thread);
thread_t *Dequeue(Queue *queue);


#endif // THREAD_H