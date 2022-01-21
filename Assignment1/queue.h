#ifndef __THREAD_QUEUE_H__
#define __THREAD_QUEUE_H__

#include <stdint.h>
#include "../thread/thread.h"
#include "../process/process.h"

struct _thread_node_t
{
    thread_t *thread;
    struct _thread_node_t *next;
};

struct _process_node_t
{
    process_t *process;
    struct _process_node_t *next;
};

typedef struct _thread_node_t thread_node_t;
typedef struct _process_node_t process_node_t;

typedef struct
{
    thread_node_t *head;
    thread_node_t *tail;
} thread_queue_t;

typedef struct
{
    process_node_t *head;
    process_node_t *tail;
} process_queue_t;

// Arg: Null
// Function: To create a new queue
// Return: thread_queue_t* of the newly created queue with head and tail = NULL
thread_queue_t *tq_new_queue();

// Arg: thread_queue_t* of the existing queue object
// Arg: thread_t* of the thread_t struct to be enqueued to the queue
// Function: to enqueue a thread to the queue
// Return: void
void tq_enqueue(thread_queue_t *, thread_t *);

// Arg: thread_queue_t* of the existing queue object from which dequeu is to be done;
// Function: to dequeue the top thread from the queue
// Return: thread_t*
thread_t* tq_dequeue(thread_queue_t *);

// Arg: thread_queue_t* of the queue to traverse
// Function: to traverse a queue
// Return: void
void tq_traverse(thread_queue_t *);

// Arg: thread_queue_t* of the queue to empty
// Function: empties the queue
// Returns: void
void tq_delete_queue(thread_queue_t *);

// Arg: uint64_t tid
// Function: Checks if there exists a thread in the queue with the tid passed as argument
// Returns: 0 if the tid doesn't exist and 1 if the tid exists
int tq_tid_exists(thread_queue_t *, uint64_t);

// Arg: thread_queue_t* of the given queue and uint64_t tid
// Function: returns and dequeues the thread with given tid from the queue
// Return: thread_t* of the given tid
thread_t* tq_dequeue_tid(thread_queue_t*, uint64_t);

// Arg: thread_queue_t* of the queue
// Function: tells if the queue is empty or not
// Return: 0 if it is not empty othervise 1
int tq_empty(thread_queue_t *);

// Arg: Null
// Function: To create a new process queue
// Return: process_queue_t* of the newly created queue with head and tail = NULL
process_queue_t *pq_new_queue();

// Arg: process_queue_t* of the existing queue object
// Arg: process_t* of the thread_t struct to be enqueued to the queue
// Function: to enqueue a process to the queue
// Return: void
void pq_enqueue(process_queue_t *, process_t *);

// Arg: process_queue_t* of the existing queue object from which dequeue is to be done;
// Function: to dequeue the top process from the queue
// Return: process_t*
process_t* pq_dequeue(process_queue_t *);

// Arg: process_queue_t* of the queue to traverse
// Function: to traverse a queue
// Return: void
void pq_traverse(process_queue_t *);

// Arg: process_queue_t* of the queue to empty
// Function: empties the queue
// Returns: void
void pq_delete_queue(process_queue_t *);

// Arg: uint64_t pid
// Function: Checks if there exists a process in the queue with the pid passed as argument
// Returns: 0 if the pid doesn't exist and 1 if the pid exists
int pq_pid_exists(process_queue_t*, uint64_t);

// Arg: process_queue_t* of the given queue and uint64_t tid
// Function: returns and dequeues the process with given pid from the queue
// Return: process_t* of the given pid
process_t* pq_dequeue_pid(process_queue_t*, uint64_t);

// Arg: process_queue_t* of the queue
// Function: tells if the queue is empty or not
// Return: 0 if it is not empty othervise 1
int pq_empty(process_queue_t *);

#endif