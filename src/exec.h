#ifndef EXEC_H
#define EXEC_H

#include <signal.h>
#include "parse.h"
#include "processlist.h"

Processlist bkg_process_list;

int exec(char ***processes, int processnum, flag *is_timeX, flag *has_pipe, flag *is_bkg);

#endif
