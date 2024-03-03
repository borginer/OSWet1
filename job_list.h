#ifndef _JOB_ARR_H
#define _JOB_ARR_H
#define _POSIX_C_SOURCE 200809L
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

/* manages all the background processes (jobs)
   including sending signals*/

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
jnode* get_job(int job_id);

// gets a stopped job with highest job_id
jnode* get_stopped_job();
//implemets bg cmd
int  bg_run(int job_id);
//implements fg cmd, waitpid happenes in commands.c
int  fg_run(int job_id, char* cmd);
//implements kill cmd
int  sig_job(int job_id, int sig);
//implements jobs cmd
void jobs_print();
// remove all zombies from jobs, called at start of every command
void sweep_zombies();
//implements quit kill cmd
void kill_jobs();

#endif
