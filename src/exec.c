#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

#include "exec.h"
#include "signal.h"
#include "parse.h"
#include "processlist.h"

int exec(char ***processes, int processnum, flag *is_timeX, flag *has_pipe, flag *is_bkg) {
    // todo: pipes
    
    if (processnum == 0) {
        char **arg = processes[0];
        pid_t pid = fork();


        if (pid < 0) {
            fprintf(stdout, "3230shell: ERROR: Fork failed\n");
            fflush(stdout);
            return -1;
        }

        else if (pid == 0) {
            while (sigusr1==0) {usleep(1000);}
            sigusr1 = 0;

            if (!*is_bkg) {
                // printf("FG child process %d\n", (int) getpid());
                signal(SIGINT, SIG_DFL);
            }
            else {
                // signal(SIGCHLD, SIG_IGN);
                signal(SIGINT, SIG_IGN);
                signal(SIGTSTP, SIG_IGN);
            }
            if (execvp(arg[0], arg) == -1) {
                fprintf(stdout, "3230shell: ERROR: Failed to execute %s: %s\n", arg[0], strerror(errno));
                fflush(stdout);
                exit(-1);
            }
        }
        // parent process
        else {
            struct tms start_time;
            times(&start_time);

            kill(pid, SIGUSR1);
            // printf("sent sigusr\n");
            if (*is_bkg) {
                add_process(&bkg_process_list, pid, arg[0]);
                // printf("BG process list : ");
                // printList(&bkg_process_list);
                // pid = 0;
                // int status;
                // pid_t child_pid = waitpid(-1, &status, WNOHANG);
                return 0;
            }
            while (1) {
                register_signal();
                int status;
                pid_t child_pid = waitpid(-1, &status, 0);
                // printf("waited child_pid %d\n", child_pid);
                if (child_pid==-1) {
                    printf("error waiting child %d\n", pid);
                    return 0;
                }
           
                // foreground child process
                if (!*is_bkg) { 
                    if (*is_timeX) {
                        struct tms end_time;
                        times(&end_time);
                        double user_time = (double) (end_time.tms_cutime - start_time.tms_cutime) / sysconf(_SC_CLK_TCK);
                        double sys_time = (double) (end_time.tms_cstime - start_time.tms_cstime) / sysconf(_SC_CLK_TCK);
                        fprintf(stdout, "(PID)%d  (CMD)%s    (user)%.3f s  (sys)%.3f s\n", pid, arg[0], user_time, sys_time);
                        fflush(stdout);
                    }
                    // printf("FG Child process (%d) exited, with exit status %d\n", 
                    //     (int) child_pid, WEXITSTATUS(status));
                    if (WIFSIGNALED(status)) {
                        if (SIGINT) {
                            fprintf(stdout, "Interrupt\n");
                            break;
                        }
                        if (SIGKILL) {
                            fprintf(stdout, "Killed\n");
                            break;
                        }
                        else {
                            fprintf(stdout, "Terminated by signal %d\n", WIFSIGNALED(status));
                            break;
                        }
                    }
                    return 0;
                }
            }
        }
    }
}


