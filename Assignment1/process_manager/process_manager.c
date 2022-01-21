#include "process_manager.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

process_manager_t *new_process_manager()
{
    process_manager_t *pm = (process_manager_t *)malloc(sizeof(process_manager_t));
    pm->processes = pq_new_queue();
    pm->new_queue = tq_new_queue();
    pm->ready_queue = tq_new_queue();
    pm->processor1_thread = NULL;
    pm->processor2_thread = NULL;
    pm->terminated_queue = tq_new_queue();
    pm->waiting_queue = tq_new_queue();
    return pm;
}

void delete_process_manager(process_manager_t *pm)
{
    assert(pm != NULL);
    tq_delete_queue(pm->waiting_queue);
    tq_delete_queue(pm->new_queue);
    tq_delete_queue(pm->ready_queue);
    tq_delete_queue(pm->terminated_queue);
    pq_delete_queue(pm->processes);
    if (pm->processor1_thread != NULL)
        delete_thread(pm->processor1_thread);
    if (pm->processor2_thread != NULL)
        delete_thread(pm->processor2_thread);
    free(pm);
}

uint64_t create_process(process_manager_t *pm, uint64_t pid, uint64_t psw, uint64_t reg0, uint64_t reg1, uint64_t reg2)
{
    assert(pm != NULL);
    if (pq_pid_exists(pm->processes, pid))
    {
        fputs("PID already exists", stderr);
        return -1;
    }
    process_t *proc = new_proces();
    proc->pid = pid;
    proc->psw = psw;
    proc->reg0 = reg0;
    proc->reg1 = reg1;
    proc->reg2 = reg2;
    pq_enqueue(pm->processes, proc);
    return pid;
}

uint64_t create_thread(process_manager_t *pm, uint64_t tid, uint64_t pid, uint64_t tsw, uint64_t reg0, uint64_t reg1, uint64_t reg2)
{
    assert(pm != NULL);
    if (_check_tid_existence(pm, tid))
    {
        fputs("TID already exists\n", stderr);
        return -1;
    }
    if (!pq_pid_exists(pm->processes, pid))
    {
        fputs("Process with the given PID doesn't exists\n", stderr);
        return -1;
    }
    thread_t *thread = new_thread();
    thread->pid = pid;
    thread->reg0 = reg0;
    thread->reg1 = reg1;
    thread->reg2 = reg2;
    thread->tid = tid;
    thread->tsw = tsw;
    tq_enqueue(pm->new_queue, thread);
    return tid;
}

void list(process_manager_t *pm, const char *queue_name)
{
    if (strncmp(queue_name, "all", 3) == 0)
    {
        printf("new_queue: ");
        tq_traverse(pm->new_queue);
        printf("ready_queue: ");
        tq_traverse(pm->ready_queue);
        printf("waiting_queue: ");
        tq_traverse(pm->waiting_queue);
        printf("terminated_queue: ");
        tq_traverse(pm->terminated_queue);
        _print_cpu(pm);
        return;
    }
    if (strncmp(queue_name, "waiting", 7) == 0)
    {
        printf("waiting_queue: ");
        tq_traverse(pm->waiting_queue);
        return;
    }
    if (strncmp(queue_name, "ready", 5) == 0)
    {
        printf("ready_queue: ");
        tq_traverse(pm->ready_queue);
        return;
    }
    if (strncmp(queue_name, "new", 3) == 0)
    {
        printf("new_queue: ");
        tq_traverse(pm->new_queue);
        return;
    }
    if (strncmp(queue_name, "terminated", 10) == 0)
    {
        printf("terminated_queue: ");
        tq_traverse(pm->terminated_queue);
        return;
    }
    if (strncmp(queue_name, "SCHED", 5) == 0)
    {
        _print_cpu(pm);
        return;
    }
    fputs("QUEUE not found", stderr);
    return;
}

void go(process_manager_t *pm)
{
    if (!tq_empty(pm->ready_queue))
    {
        pm->processor1_thread = tq_dequeue(pm->ready_queue);
    }
    if (!tq_empty(pm->ready_queue))
    {
        pm->processor2_thread = tq_dequeue(pm->ready_queue);
    }
}

void unwait(process_manager_t *pm, uint64_t tid)
{
    thread_t *thread = tq_dequeue_tid(pm->waiting_queue, tid);
    if (thread == NULL)
    {
        fputs("Thread doesn't exist in wating queue", stderr);
    }
    else
    {
        tq_enqueue(pm->ready_queue, thread);
    }
}

void eoquantum(process_manager_t *pm, uint64_t tid)
{
    if (pm->processor1_thread != NULL && pm->processor1_thread->tid == tid)
    {
        tq_enqueue(pm->ready_queue, pm->processor1_thread);
        pm->processor1_thread = NULL;
        return;
    }
    if (pm->processor2_thread != NULL && pm->processor2_thread->tid == tid)
    {
        tq_enqueue(pm->ready_queue, pm->processor2_thread);
        pm->processor2_thread = NULL;
        return;
    }
    fprintf(stderr, "TID: %lu is currently not scheduled", tid);
}

void wait(process_manager_t *pm, uint64_t tid)
{
    if (pm->processor1_thread != NULL && pm->processor1_thread->tid == tid)
    {
        tq_enqueue(pm->waiting_queue, pm->processor1_thread);
        pm->processor1_thread = NULL;
        return;
    }
    if (pm->processor2_thread != NULL && pm->processor2_thread->tid == tid)
    {
        tq_enqueue(pm->waiting_queue, pm->processor2_thread);
        pm->processor2_thread = NULL;
        return;
    }
    fprintf(stderr, "TID: %lu is currently not scheduled", tid);
}

void eolife(process_manager_t *pm, uint64_t tid)
{
    if (pm->processor1_thread != NULL && pm->processor1_thread->tid == tid)
    {
        tq_enqueue(pm->terminated_queue, pm->processor1_thread);
        pm->processor1_thread = NULL;
        return;
    }
    if (pm->processor2_thread != NULL && pm->processor2_thread->tid == tid)
    {
        tq_enqueue(pm->terminated_queue, pm->processor2_thread);
        pm->processor2_thread = NULL;
        return;
    }
    fprintf(stderr, "TID: %lu is currently not scheduled", tid);
}

void ready(process_manager_t *pm)
{
    if (tq_empty(pm->new_queue))
    {
        fputs("ERROR: no thread in the new_queue", stderr);
        return;
    }
    tq_enqueue(pm->ready_queue, tq_dequeue(pm->new_queue));
}

int _check_tid_existence(process_manager_t *pm, uint64_t tid)
{
    if (tq_tid_exists(pm->ready_queue, tid))
    {
        return 1;
    }
    if (tq_tid_exists(pm->new_queue, tid))
    {
        return 1;
    }
    if (tq_tid_exists(pm->waiting_queue, tid))
    {
        return 1;
    }
    if (tq_tid_exists(pm->terminated_queue, tid))
    {
        return 1;
    }

    if (pm->processor1_thread != NULL && pm->processor1_thread->tid == tid)
    {
        return 1;
    }

    if (pm->processor2_thread != NULL && pm->processor2_thread->tid == tid)
    {
        return 1;
    }
    return 0;
}

void _print_cpu(process_manager_t *pm)
{
    if (pm->processor1_thread != NULL)
    {
        printf("core 1 : |pid = %lu; tid = %lu|\n", pm->processor1_thread->pid, pm->processor1_thread->tid);
    }
    else
    {
        printf("core 1 : NULL\n");
    }
    if (pm->processor2_thread != NULL)
    {
        printf("core 2 : |pid = %lu; tid = %lu|\n", pm->processor2_thread->pid, pm->processor2_thread->tid);
    }
    else
    {
        printf("core 2 : NULL\n");
    }
}
