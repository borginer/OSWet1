#ifndef _SIGS_H
#define _SIGS_H
#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "commands.h"
#include "job_list.h"

void pause_handler();
void kill_handler();
#endif

