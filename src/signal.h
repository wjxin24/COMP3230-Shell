#ifndef SIGNAL_H
#define SIGNAL_H

volatile int sigusr1;

void register_signal();
void sigint_handler(int signum);
void sigtstp_handler(int signum);
void sigusr1_handler(int signum);
void sigchld_handler(int signum);
#endif
