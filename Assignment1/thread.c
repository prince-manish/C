#include "thread.h"
#include <stdlib.h>
#include <assert.h>

// Function: malloc a new thread and return its pointer
// Return: thread_t* of the newly created thread
thread_t *new_thread()
{
    return (thread_t *)malloc(sizeof(thread_t));
}

// Arg: thread_t* of the thread to delete
// Function: delete the thread struct and free it's memory
// Return: void
void delete_thread(thread_t* thread)
{
    assert(thread != NULL);
    free(thread);
}