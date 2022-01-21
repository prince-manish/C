#include <stdio.h>
#include "queue.h"
#include <stdlib.h>
#include <assert.h>
#include "../thread/thread.h"

thread_queue_t *tq_new_queue()
{
    thread_queue_t *queue = (thread_queue_t *)malloc(sizeof(thread_queue_t));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void tq_enqueue(thread_queue_t *queue, thread_t *thread)
{
    thread_node_t *thread_node = (thread_node_t *)malloc(sizeof(thread_node_t));
    thread_node->thread = thread;
    if (queue->tail == NULL)
    {
        queue->head = thread_node;
        queue->tail = thread_node;
    }
    else
    {
        queue->tail->next = thread_node;
        queue->tail = thread_node;
    }
}

thread_t *tq_dequeue(thread_queue_t *queue)
{ 
    // queue->head cannot be null
    assert(queue->head != NULL);
    thread_node_t *temp = queue->head;
    thread_t *thread = temp->thread;
    queue->head = temp->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    free(temp);
    return thread;
}

void tq_traverse(thread_queue_t *queue)
{
    for (thread_node_t *current = queue->head; current != NULL; current = current->next)
    {
        printf("|thread_id = %lu; pid = %lu| --> ", current->thread->tid, current->thread->pid);
    }
    puts("NULL\n");
}

void tq_delete_queue(thread_queue_t *queue)
{
    assert(queue != NULL);

    for (thread_node_t *current = queue->head; current != NULL;)
    {
        thread_node_t *temp = current;
        current = temp->next;
        delete_thread(temp->thread);
        free(temp);
    }
    free(queue);
}

process_queue_t *pq_new_queue()
{
    process_queue_t *queue = (process_queue_t *)malloc(sizeof(process_queue_t));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void pq_enqueue(process_queue_t *queue, process_t *process)
{
    process_node_t *process_node = (process_node_t *)malloc(sizeof(process_node_t));
    process_node->process = process;
    if (queue->tail == NULL)
    {
        queue->head = process_node;
        queue->tail = process_node;
    }
    else
    {
        queue->tail->next = process_node;
        queue->tail = process_node;
    }
}

process_t *pq_dequeue(process_queue_t *queue)
{
    // queue->head cannot be null
    assert(queue->head != NULL);
    process_node_t *temp = queue->head;
    process_t *proc = temp->process;
    queue->head = temp->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    free(temp);
    return proc;
}

void pq_traverse(process_queue_t *queue)
{
    for (process_node_t *current = queue->head; current != NULL; current = current->next)
    {
        printf("|pid = %lu| --> ", current->process->pid);
    }
    puts("NULL\n");
}

void pq_delete_queue(process_queue_t *queue)
{
    assert(queue != NULL);

    for (process_node_t *current = queue->head; current != NULL;)
    {
        process_node_t *temp = current;
        current = temp->next;
        delete_process(temp->process);
        free(temp);
    }
    free(queue);
}

int pq_pid_exists(process_queue_t *pq, uint64_t pid)
{
    for (process_node_t *current = pq->head; current != NULL; current = current->next)
    {
        if (current->process->pid == pid)
            return 1;
    }
    return 0;
}

int tq_tid_exists(thread_queue_t *tq, uint64_t tid)
{
    for (thread_node_t *current = tq->head; current != NULL; current = current->next)
    {
        if (current->thread->tid == tid)
            return 1;
    }
    return 0;
}

process_t *pq_head(process_queue_t *pq)
{
    assert(pq != NULL);
    return pq->head->process;
}

thread_t *tq_head(thread_queue_t *tq)
{
    assert(tq != NULL);
    return tq->head->thread;
}

thread_t *tq_dequeue_tid(thread_queue_t *tq, uint64_t tid)
{
    thread_t *ret;
    if (tq->head->thread->tid == tid)
    {
        return tq_dequeue(tq);
    }
    for (thread_node_t *current = tq->head; current->next != NULL; current = current->next)
    {
        if (current->next->thread->tid == tid)
        {
            ret = current->next->thread;
            current->next = current->next->next;
            return ret;
        }
    }
    return NULL;
}

process_t *pq_dequeue_pid(process_queue_t *pq, uint64_t pid)
{
    process_t *ret;
    if (pq->head->process->pid == pid)
    {
        return pq_dequeue(pq);
    }
    for (process_node_t *current = pq->head; current->next != NULL; current = current->next)
    {
        if (current->next->process->pid == pid)
        {
            ret = current->next->process;
            current->next = current->next->next;
            return ret;
        }
    }
    return NULL;
}

int tq_empty(thread_queue_t *tq) {
    return tq->head == NULL;
}

int pq_empty(process_queue_t *pq) {
    return pq->head == NULL;
}