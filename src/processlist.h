#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Process{
    pid_t pid;
    char *arg0;
    struct Process *next;
};
typedef struct Process Process;

struct Processlist {
    Process *head;
    Process *current;
};
typedef struct Processlist Processlist;

void add_process(Processlist *process_list, pid_t pid, char *arg0);
Process* find_pid(Processlist *process_list, pid_t pid);
bool delete_process(Processlist *process_list, pid_t pid);

#endif
