// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

//Using helper functions from commands.c to get running process info

//ctrl +Z handler
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
//ctrl +C handler
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

void set_pause_handler(){
   struct sigaction act;
   memset((void*)&act, 0, sizeof(act));
	act.sa_handler = &pause_handler;
	sigaction(SIGTSTP, &act, NULL);
}

void set_kill_handler(){
   struct sigaction act;
   memset((void*)&act, 0, sizeof(act));
	act.sa_handler = &kill_handler;
	sigaction(SIGINT, &act, NULL);
}
