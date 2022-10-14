#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

#include "processlist.h"
#include "signal.h"
#include "exec.h"

void register_signal() {
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGCHLD, sigchld_handler);
}

void sigint_handler(int signum) {
    fprintf(stdout, "\n$$ 3230shell ## ");
    fflush(stdout);
}

void sigtstp_handler(int signum) {
    fprintf(stdout, "\n$$ 3230shell ## ");
    fflush(stdout);
}

void sigusr1_handler(int signum) {
    sigusr1 = 1;
}

void sigchld_handler(int signum) {
    int status;
    pid_t pid = waitpid(0, &status, WNOHANG);
    if (pid<0) return;
    if (find_pid(&bkg_process_list, pid)) {
        if (status==0) {
            fprintf(stdout, "\n%s Done\n$$ 3230shell ## ", find_pid(&bkg_process_list, pid)->arg0);
            fflush(stdout);
        }
        else if (WIFSIGNALED(status)) {
            fprintf(stdout, "\n%s Stopped by signal %s\n$$ 3230shell ## ", find_pid(&bkg_process_list, pid)->arg0, strsignal(WTERMSIG(status)));
            fflush(stdout);
        }
        delete_process(&bkg_process_list, pid);
    }
}