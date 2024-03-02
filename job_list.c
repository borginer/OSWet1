#include "job_list.h"

#define MAX_LINE_SIZE 80

//Used exlusively by shell main process
jnode *head;

bool jobs_init(){
    head = (jnode*)malloc(sizeof(jnode));
    if(!head)
        return false;
    head->next = head;
    head->prev = head;
    head->job_id = 0;
    return true;
}

 bool jobs_add(int type, int pid, char *cmd){
    jnode* node = (jnode*)malloc(sizeof(jnode));
    if(!node)
        return false;

    node->prev = head->prev;
    node->next = head;
    head->prev = node;
    node->prev->next = node;

    node->job_id = node->prev->job_id + 1;
    node->type = type;
    node->pid = pid;
    node->cmd = (char*)malloc(strlen(cmd) + 1);
    if(!node->cmd){
        free(node);
        return false;
    }

    strcpy(node->cmd, cmd);
    node->start = time(NULL);
    return true;
}

bool jobs_remove(int job_id){
    jnode* p = head;
    while((p = p->next) != head){
        if(p->job_id == job_id){
            p->next->prev = p->prev;
            p->prev->next = p->next;
            free(p->cmd);
            free(p);
            return true;
        }
    }
    return false;
}

jnode* get_job(int job_id){
    jnode* p = head;
    while((p = p->next) != head){
        if(p->job_id == job_id)
            return p;
    }
    return NULL;
}

jnode* get_stopped_job(){
    jnode* p = head;
    while((p = p->prev) != head){
        if(p->type == stopped)
            return p;
    }
    return NULL;
}

jnode* get_top_job(){
    if(head->prev != head)
        return head->prev;
    return NULL;
}

void jobs_print(){
    jnode* p = head->next;
    double time_diff;
    while(p != head){
        time_diff = difftime(time(NULL), p->start);
        if(p->type == stopped)
            printf("[%d] %s : %d %.0f secs (stopped)\n", p->job_id, p->cmd, p->pid, time_diff);
        else
            printf("[%d] %s : %d %.0f secs\n", p->job_id, p->cmd, p->pid, time_diff);
        p = p->next;
    };
}

void sweep_zombies(){
    if(head == head->next)
        return;
    jnode* p = head;
    int stat, job_id;
    pid_t pid;
    while((p = p->next) != head){
        if((pid = waitpid(p->pid, &stat, WNOHANG)) == -1)
            PRINT_SYS_ERROR(waitpid);
        if(pid){
            job_id = p->job_id;
            p = p->next;
            jobs_remove(job_id);
        }
            
    }
}

void kill_jobs(){
    if(head == head->next)
        return;
    jnode* p = head;
    int stat, count;
    pid_t pid;

    p = p->next;
    while(p != head){
        count = 0;
        printf("[%d] %s - Sending SIGTERM... ", p->job_id, p->cmd);
        fflush(stdout);
        if(kill(p->pid, SIGTERM))
            PRINT_SYS_ERROR(kill);
        for(; (pid = waitpid(p->pid, &stat, WNOHANG)) == 0 && count < 5; count++){
            sleep(1);
        }
        if(pid == -1){PRINT_SYS_ERROR(waitpid);}
        if(count < 5){
            printf("Done.\n");
        }
        else{
            printf("(5 sec passed) Sending SIGKILL... ");
            fflush(stdout);
            if(kill(p->pid, SIGKILL))
                PRINT_SYS_ERROR(kill);
            printf("Done.\n");

        }
        int job_id = p->job_id;
        p = p->next;
        jobs_remove(job_id);
    }
}

int sig_job(int job_id, int sig){
    jnode* job;
    if((job = get_job(job_id))){
        if(kill(job->pid, sig)){
            PRINT_SYS_ERROR(kill);
            return jobs_error;
        }
        else{
            return job->pid;
        }
    }
    return job_not_found;
}

int bg_run(int job_id){
    jnode* job;
    if(job_id != 0 && (job = get_job(job_id))){
        if(job->type != stopped){
            return wrong_type;
        } 
    }
    else{
        if(job_id != 0){
            return job_not_found;
        }
        if(!(job = get_stopped_job())){
            return no_stopped_jobs;
        }
    }
    if(kill(job->pid, SIGCONT)){
        PRINT_SYS_ERROR(kill);
        return jobs_error;
    }
    job->type = background;
    double time_diff = difftime(time(NULL), job->start);
    printf("%s : %.0f\n", job->cmd, time_diff);
    return jobs_success;
}

int fg_run(int job_id, char* cmd){
    if(head->next == head)
        return no_jobs;
    jnode* job;
    if(job_id != 0 && !(job = get_job(job_id))){
        return job_not_found;
    }
    else if(!(job = get_top_job())){
        return no_jobs;
    }
    
    double time_diff = difftime(time(NULL), job->start);
    printf("%s : %.0f\n", job->cmd, time_diff);
    if(kill(job->pid, SIGCONT)){
        PRINT_SYS_ERROR(kill);
        return jobs_error;
    }
    
    strcpy(cmd, job->cmd);
    int pid = job->pid;
    jobs_remove(job->job_id);
    return pid;
}