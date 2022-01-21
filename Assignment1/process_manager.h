#ifndef __PROCESS_MANAGER_H__
#define __PROCESS_MANAGER_H__

#include <stdint.h>
#include "../queue/queue.h"
#include "../process/process.h"
#include "../thread/thread.h"

typedef struct {
    // queues for scheduling
    thread_t *processor1_thread;
    thread_t *processor2_thread;
    thread_queue_t *ready_queue;
    thread_queue_t *waiting_queue;
    thread_queue_t *new_queue;
    thread_queue_t *terminated_queue;
    process_queue_t *processes;
} process_manager_t;


// Arg: void
// Function: Create a new empty process manager
// Return: process_manager_t*
process_manager_t *new_process_manager();

// Arg: process_manager_t* of the process_manager to delete
// Function: Delete process manager and free all the memory
// Return: void
void delete_process_manager(process_manager_t*);

// Initialization commands

// Arg: process_manager_t*, pid, psw, reg0, reg1, reg2
// Function: The created PIDs must be unique. If this command requires you to perform multiple actions within your
// PM, then this one directive will cause those actions to happen. The result of this directive will be that
// there is a new PCB in the Ready queue and that PID will be displayed back to the test interface.
// Return uint64_t pid
uint64_t create_process(process_manager_t*, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

// Arg: process_manager_t*, tid, pid, tsw, reg0, reg1, reg2
// Function: The created TIDs must be unique.  The result of this directive will be that
// there is a new TCB in the Ready queue and that TID will be displayed back to the test interface.
// Return uint64_t tid
uint64_t create_thread(process_manager_t*, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

// Status commands

// Arg: process_manager_t* process manager, and char* for the queue_name
// Function: to list the provided queue {queue_name | all | SCHED}
// Should display the selected queue, all the process queues, or the PID of the process that is
// scheduled next, when this command is executed.
// Return: void
void list(process_manager_t*, const char*);

// Action command

// Arg: process_manager_t* for process_manager
// Function: Use of this command triggers running of the scheduler module; currently this command will
// dequeue two threads from ready queue and assign the processor cores to them
// Return: void
void go(process_manager_t*);

// Transition commands

// Arg: process_manager_t* process_manager, and uint64_t thread id
// Function: This command moves a thread from the WAIT queue to the ready queue
// Return: void
void unwait(process_manager_t*, uint64_t);

// Arg: process_manager_t* of process manager and uint64_t thread id
// Function: Moves the current Running thread with the given tid back to the Ready queue as if its time
// quantum has expired
// Return: void
void eoquantum(process_manager_t*, uint64_t);

// Arg: process_manager_t* of process manager and uint64_t thread id
// Function: Moves the current Running thread with the given tid to the Terminated queue as if it’s asked to quit
// Return: void
void eolife(process_manager_t*, uint64_t);

// Arg: process_manager_t* of process_manager and uint64_t thread id
// Function: Moves the current Running thread with the given tid to the Waiting queue as if it performed
// some I/O and needs to wait for it to complete
// Return: void
void wait(process_manager_t*, uint64_t);

// Arg: process_manager_t* of process manager
// Function: Moves a thread from new_queue to ready_queue
// Return: void
void ready(process_manager_t*);

// Arg: process_manager_t* of process manager
// Function: checks if a thread of the passed tid exists
// Return: 0 if thread doesn't exist 1 otherwise
int _check_tid_existence(process_manager_t*, uint64_t);

// Arg: process_manager_t* for the process manager
// Function: internal function to print the processor status
// Return void
void _print_cpu(process_manager_t*);

#endif