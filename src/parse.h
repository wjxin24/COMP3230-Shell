#ifndef PARSE_H
#define PARSE_H

// typedef struct Process {
//     struct Process *next; // pointer to the next process after a pipe
//     int ispipe; // if the process is in a pipe
//     char **args; // arguments of the process command
    
// } Process;

#define MAX_PROCESS_NUM 5
#define MAX_STRING_NUM 30
#define MAX_CHAR_NUM 1024

#define ERROR -2
#define EXIT -1

typedef int flag;

int parseline(char *input, char ***processes, flag  *is_timeX, flag *has_pipe, flag *is_bkg);

#endif
