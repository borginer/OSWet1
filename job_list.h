#ifndef _JOB_ARR_H
#define _JOB_ARR_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define MAX_JOBS 100

typedef struct jnode{
    int type;
    int job_num;
    pid_t pid;
    time_t start;
    struct jnode* next;
} jnode;

void jobs_init();
void jobs_add(int _type, int _pid);

#endif
