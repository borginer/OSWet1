// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

void pause_handler(){
   printf("smash: caught ctrl-Z\n");
   if(IsRunning()){
      if(kill(RunningPID(), SIGSTOP))
         PRINT_SYS_ERROR(kill);
      else{
         jobs_add(stopped, RunningPID(), RunningCMD());
         printf("smash: process %d was stopped\n", RunningPID());
      }
   }
}

void kill_handler(){
   printf("smash: caught ctrl-C\n");
   if(IsRunning()){
      if(kill(RunningPID(), SIGKILL))
         PRINT_SYS_ERROR(kill);
      else{
         printf("smash: process %d was killed\n", RunningPID());
      }
   }
}
