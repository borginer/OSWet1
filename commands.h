#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "job_list.h"

#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define PRINT_SYS_ERROR(name) do {perror("smash error: " #name " failed\n");} while(0)

int BgCmd(char* lineSize);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString, int bg);
int FilesDiff(char* name1, char* name2);
bool IsRunning();
int RunningPID();
char* RunningCMD();
#endif
