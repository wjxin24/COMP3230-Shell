/************************************************************
* Student name and No.: 
* Development platform: WSL Ubuntu
* Remark: 
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#include "readline.h"
#include "signal.h"
#include "parse.h"
#include "exec.h"
#include "processlist.h"


int main() {
    while (1) {
        register_signal();
        setbuf(stdout, NULL);
        fprintf(stdout, "$$ 3230shell ## ");
        fflush(stdout);
        char* input = readline();
        if (*input=='\0') continue;
        flag is_timeX = 0;
        flag has_pipe = 0;
        flag is_bkg = 0;
        char ***processes;
        processes = calloc(MAX_PROCESS_NUM, sizeof(char**));
        int parse_result = parseline(input, processes, &is_timeX, &has_pipe, &is_bkg); 
        if (parse_result == ERROR) continue;
        if (parse_result == EXIT) exit(0);
        if (parse_result >= 0) {
            int exec_result = exec(processes, parse_result, &is_timeX, &has_pipe, &is_bkg); 
        }

    }    

    return 0;
}