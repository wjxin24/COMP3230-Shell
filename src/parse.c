#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

int parseline(char *input, char ***processes, flag *is_timeX, flag *has_pipe, flag *is_bkg) {
    int processnum = 0;
    int argc = 0;
    processes[processnum] = malloc(strlen(input)*sizeof(char*));
    char* modified_input;
    modified_input = malloc(strlen(input)*sizeof(char));
    strcpy(modified_input, input);
    
    // check background process
    if (modified_input[strlen(input)-1] == '&') {
        *is_bkg = 1;
        modified_input[strlen(input)-1] = '\0';
        int bk = '&';
        char *p_bk = strchr(modified_input, bk);
        if (p_bk != NULL) {
            printf("3230shell: '&' should not appear in the middle of the command line\n");
            return ERROR;
        }
    }
   
    const char space[2] = " ";
    char *p = strtok(modified_input, space);
    if (p[0]=='|') {
        printf("3230shell: should not have | as the first character on the input line\n");
        return ERROR;
    }
    while (p!=NULL) {
        if (p[0]!='&' && p[0]!='|') {
            processes[processnum][argc] = (char *) malloc(strlen(p)*sizeof(char));
            processes[processnum][argc++] = p;
        }
        
        if (p[0]=='|') {
            // read the command after the pipe
            p = strtok(NULL, space);
            if (p==NULL) {
                printf("3230shell: should not have | as the last character on the input line\n");
                return ERROR;
            }
            if (p[0]=='|') {
                printf("3230shell: should not have two consecutive | without in-between command\n");
                return ERROR;
            }
            processes[++processnum] = malloc(strlen(p)*sizeof(char*));
            if (processnum == MAX_PROCESS_NUM) {
                printf("3230shell: should not have more than 4 pipes\n");
                return ERROR;
            }
            argc=0;
            processes[processnum][argc++] = p; 
        }
        p = strtok(NULL, space);
    }
    
    if (strcmp(processes[0][0], "exit")==0) {
        if (processnum == 0 && processes[0][1]==NULL && !*is_bkg) {
            printf("3230shell: Terminated\n");
            return EXIT;
        }
        else {
            printf("3230shell: \"exit\" with other arguments!!!\n");
            return ERROR;
        }
    }

    if (strcmp(processes[0][0], "timeX")==0) {
        if (processes[0][1]==NULL) {
            printf("3230shell:  \"timeX\" cannot be a standalone command\n");
            return ERROR;
        }
        else {
            *is_timeX = 1;
            processes[0] = &processes[0][1];
        }
    }

    if (processnum>0) *has_pipe = 1;
    
    if (*is_timeX && *is_bkg) {
        printf("3230shell: \"timeX\" cannot be run in background mode\n");
        return ERROR;
    }

    return processnum;
}

