#include "processlist.h"


void add_process(Processlist *process_list, pid_t pid, char *arg0) {
    Process *bkg_process = (Process*) malloc(sizeof(Process));
    bkg_process->pid = pid;
    bkg_process->arg0 = arg0;
    bkg_process->next = process_list->head;
    process_list->head = bkg_process;
}

Process* find_pid(Processlist *process_list, pid_t pid) {
    Process* current = process_list->head;
    if (process_list->head == NULL) {
        return NULL;
    }
    while(current->pid != pid) {
        if(current->next == NULL) {
            return NULL;
        } else {
            current = current->next;
        }
    }      
    return current;
}

bool delete_process(Processlist *process_list, pid_t pid) {
    Process* current = process_list->head;
    Process* previous = NULL;
    if (process_list->head == NULL) {
        return false;
    }
    while(current->pid != pid) {
      if (current->next == NULL) {
            return false;
        } else {
            previous = current;
            current = current->next;
        }
    }
    if (current == process_list->head) {
        process_list->head = process_list->head->next;
    } else {
        previous->next = current->next;
    }
    return true;
}
