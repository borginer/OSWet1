#include "job_list.h"

jnode *head;
jnode *tail;
int max_num;

void jobs_init(){
    max_num = 0;
    head = NULL;
    tail = NULL;
}

void jobs_add(int _type, int _pid){
    if(!head){
        head = (jnode*)malloc(sizeof(head));
        tail = head;
    } 
    else{
        jnode* node = (jnode*)malloc(sizeof(node));
        tail->next = node;
        tail = tail->next;
    }
    tail->type = _type;
    tail->job_num = max_num + 1;
    max_num += 1;
    tail->pid = _pid;
    tail->next = NULL;
    tail->start = time(NULL);
}

void jobs_remove(int _job_num){

}