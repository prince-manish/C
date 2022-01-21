#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdint.h>

typedef struct {
    uint64_t tid;
    uint64_t pid;
    uint64_t tsw;
    uint64_t reg0;
    uint64_t reg1;
    uint64_t reg2;
} thread_t;

// Arg: void
// Function: to create a new thread
// Return: thread_t* of the newly created thread
thread_t *new_thread();

// Arg: thread_t*
// Function: to delete a thread and free all its memory
// Return: void
void delete_thread(thread_t *thread);

#endif