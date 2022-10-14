#ifndef EXEC_H
#define EXEC_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <unistd.h>

#include "parse.h"
#include "processlist.h"

Processlist bkg_process_list;

int exec(char ***processes, int processnum, flag *is_timeX, flag *has_pipe, flag *is_bkg);
int exec_process(char **arg, flag *is_timeX, flag *is_bkg, int in, int out);

#endif
