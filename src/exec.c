#include "exec.h"
#include "signal.h"
#include "parse.h"
#include "processlist.h"

int exec(char ***processes, int processnum, flag *is_timeX, flag *has_pipe, flag *is_bkg) {
    int in = 0, out = 1;
    int pfd[2];
    for (int i = 0; i<=processnum; i++) {
        if (i!=processnum) {
            pipe(pfd);
            out = pfd[1];
        } else out=1;
        if (exec_process(processes[i], is_timeX, is_bkg, in, out)==-1) return -1;
            in = pfd[0];
    }
    return 0;
}

int exec_process(char **arg, flag *is_timeX, flag *is_bkg, int in, int out) {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stdout, "3230shell: ERROR: Fork failed\n");
        fflush(stdout);
        return -1;
    }
    // child process
    else if (pid == 0) {
        if (in != 0) {
            dup2(in, 0);
            close(in);
        }
        if (out != 1) {
            dup2(out, 1);
            close(out);
        }

        if (!*is_bkg) {
            signal(SIGINT, SIG_DFL);
        }
        else {
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            // stop background process from reading stdin
            if (in == 0) close(0);
        }

        while (sigusr1==0) {usleep(1000);}
        sigusr1 = 0;

        if (execvp(arg[0], arg) == -1) {
            fprintf(stdout, "3230shell: ERROR: Failed to execute %s: %s\n", arg[0], strerror(errno));
            fflush(stdout);
            exit(-1);
        }
    }
    // parent process
    else {
        // reregister signal for safety
        register_signal();

        if (in != 0) {
            close(in);
        }
        if (out != 1) {
            close(out);
        }

        // record start time
        struct tms start_time;
        times(&start_time);

        kill(pid, SIGUSR1);

        if (*is_bkg) {
            add_process(&bkg_process_list, pid, arg[0]);
            return 0;
        }

        while (1) {
            register_signal();
            int status;
            pid_t child_pid = waitpid(-1, &status, 0);
            if (child_pid==-1) {
                printf("error waiting child %d\n", pid);
                return -1;
            }
        
            // foreground child process
            if (!find_pid(&bkg_process_list, pid)) { 
                if (*is_timeX) {
                    struct tms end_time;
                    times(&end_time);
                    double user_time = (double) (end_time.tms_cutime - start_time.tms_cutime) / sysconf(_SC_CLK_TCK);
                    double sys_time = (double) (end_time.tms_cstime - start_time.tms_cstime) / sysconf(_SC_CLK_TCK);
                    fprintf(stdout, "(PID)%d  (CMD)%s    (user)%.3f s  (sys)%.3f s\n", pid, arg[0], user_time, sys_time);
                    fflush(stdout);
                }
                if (WIFSIGNALED(status)) {
                    printf("%s\n", strsignal(WTERMSIG(status)));
                    break;
                }
                return 0;
            }
        }
        return 0;
        
    }
}


