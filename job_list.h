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
#include <stdbool.h>
#include "commands.h"


#define MAX_JOBS 100

enum{
    stopped = 0,
    background,
};

enum{
    no_jobs = -4,
    no_stopped_jobs = -3,
    wrong_type = -2,
    job_not_found = -1,
    jobs_error = 0,
    jobs_success = 1,
};

typedef struct jnode{
    int type;
    int job_id;
    pid_t pid;
    time_t start;
    char* cmd;
    struct jnode* next;
    struct jnode* prev;
} jnode;

bool jobs_init();
bool jobs_add(int type, int pid, char* cmd);
bool jobs_remove(int job_id);
int  bg_run(int job_id);
int  fg_run(int job_id, char* cmd);
int  sig_job(int job_id, int sig);
jnode* get_job(int job_id);
jnode* get_stopped_job();
void jobs_print();
void sweep_zombies();
void kill_jobs();



#endif
