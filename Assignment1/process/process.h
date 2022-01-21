#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include "../thread/thread.h"

typedef struct
{
    uint64_t pid;
    uint64_t psw;
    uint64_t reg0;
    uint64_t reg1;
    uint64_t reg2;
} process_t;

// Arg: void
// Function: to create a new process
// Return: process_t* of the process created 
process_t *new_proces();

// Arg: process_t*
// Function: to delete a process and free its memory
// Return: void
void delete_process(process_t*);

#endif