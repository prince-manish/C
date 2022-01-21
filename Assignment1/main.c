#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue/queue.h"
#include "thread/thread.h"
#include "process/process.h"
#include "process_manager/process_manager.h"

void print_help();

int main()
{
    char cmd[1024];
    process_manager_t *pm = new_process_manager();
    uint64_t tid = 0, pid = 0, tsw = 0, psw = 0, reg0 = 0, reg1 = 0, reg2 = 0;
    while (1)
    {
        puts("\nEnter command or help for help menu");
        printf("> ");
        fgets(cmd, 1023, stdin);
        if (strncmp(cmd, "help", 4) == 0)
        {
            print_help();
            continue;
        }
        if (strncmp(cmd, "exit", 4) == 0)
        {
            delete_process_manager(pm);
            return 0;
        }
        if (strncmp(cmd, "create_process", 14) == 0)
        {
            sscanf(cmd + 15, "%lu %lu %lu %lu %lu\n", &pid, &psw, &reg0, &reg1, &reg2);
            pid = create_process(pm, pid, psw, reg0, reg1, reg2);
            if (pid != -1)
            {
                printf("%lu\n", pid);
            }
            continue;
        }
        if (strncmp(cmd, "create_thread", 13) == 0)
        {
            sscanf(cmd + 14, "%lu %lu %lu %lu %lu %lu\n", &tid, &pid, &tsw, &reg0, &reg1, &reg2);
            tid = create_thread(pm, tid, pid, tsw, reg0, reg1, reg2);
            if (tid != -1)
            {
                printf("%lu\n", tid);
            }
            continue;
        }
        if (strncmp(cmd, "list", 4) == 0)
        {
            list(pm, cmd + 5);
            continue;
        }
        if (strncmp(cmd, "go", 2) == 0)
        {
            go(pm);
            continue;
        }
        if (strncmp(cmd, "unwait", 6) == 0)
        {
            sscanf(cmd + 7, "%lu", &tid);
            unwait(pm, tid);
            continue;
        }
        if (strncmp(cmd, "eoquantum", 9) == 0)
        {
            sscanf(cmd + 10, "%lu", &tid);
            eoquantum(pm, tid);
            continue;
        }
        if (strncmp(cmd, "eolife", 6) == 0)
        {
            sscanf(cmd + 7, "%lu", &tid);
            eolife(pm, tid);
            continue;
        }
        if (strncmp(cmd, "wait", 4) == 0)
        {
            sscanf(cmd + 5, "%lu", &tid);
            wait(pm, tid);
            continue;
        }
        if (strncmp(cmd, "ready", 5) == 0) {
            ready(pm);
            continue;
        }
        fputs("Invalid command", stderr);
    }
}

void print_help()
{
    puts("The following commands are currenlty supported");
    puts("create_process pid psw reg0 reg1 reg2 ;to create a process");
    puts("create_thread tid pid tsw reg0 reg1 reg2 ;to create a thread");
    puts("list {queue_name | all | SCHED} ;to list queues");
    puts("go ;to execute scheduler");
    puts("unwait tid ;to move a thread with tid to the ready queue from waiting_queue");
    puts("eoquantum tid ;to move the current running thread back to ready_queue");
    puts("eolife tid ;to move the current running thread to terminated_queue");
    puts("wait tid ;to move the current running thread to the waiting_queue");
    puts("ready ;to move a thread from new_queue to the ready_queue");
    puts("exit ;to exit the program");
}