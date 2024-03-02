/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "signals.h"
#include "job_list.h"
#include <signal.h>

#define MAX_LINE_SIZE 80
#define MAXARGS 20

char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
	jobs_init();
    char cmdString[MAX_LINE_SIZE];

	struct sigaction act;
	memset((void*)&act, 0, sizeof(act));
	act.sa_handler = &pause_handler;
	sigaction(SIGTSTP, &act, NULL);

	act.sa_handler = &kill_handler;
	sigaction(SIGINT, &act, NULL);

    while (1)
    {
	 	printf("smash > ");
		fflush(stdout);

		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
		ExeCmd(lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

