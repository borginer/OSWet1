//		commands.c
//********************************************
#include "commands.h"
char *last_path, *tmp;
#define PRINT_SYS_ERROR(name) perror("smash error: " #name " failed\n")
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	//char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd")) 
	{
		if(num_arg > 1){
			fprintf(stderr, "error: cd: too many arguments\n");
		} 
		else{
			tmp = getcwd(NULL, 0);
			if(!tmp){PRINT_SYS_ERROR(getcwd);}
			if(!strcmp(args[1], "-")){
				if(last_path != NULL){
					if(chdir(last_path)){PRINT_SYS_ERROR(chdir);}
				}
				else
					fprintf(stderr, "smash error: cd: OLDPWD not set\n");
			}
			else
				if(chdir(last_path)){PRINT_SYS_ERROR(chdir);}
			last_path = tmp;
		}

	} 
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char* path = getcwd(NULL, 0);
		if(!path){PRINT_SYS_ERROR(getcwd);}
		printf("%s\n", path);
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		printf("smash pid is %d\n", getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		if(num_arg == 1 && strcmp(args[1], "kill")){
			//ADD KILLING JOBS **********************************************ADD
			exit(1);
		}
		else{
			exit(0);
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
   		
	} 
	/*************************************************/	
	else if (!strcmp(cmd, "diff"))
	{
   		if(num_arg != 2)
			fprintf(stderr, "smash error: diff: invalid arguments\n");
		else
			printf("%d\n", FilesDiff(args[1], args[2]));
	} 
	/*************************************************/	
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					
					/* 
					your code
					*/
	}
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize)
{

	//changed for warnings
	// char* Command;
	// char* delimiters = " \t\n";
	// char *args[MAX_ARG];


	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

int FilesDiff(char* name1, char* name2){
	FILE* f1 = fopen(name1, "r");
	FILE* f2 = fopen(name2, "r");
	char *line1 = NULL, *line2 = NULL;
	size_t len1, len2;
	ssize_t read1, read2;
	int ans = 0;
	if(!f1 || !f2){
		fprintf(stderr, "diff: error openning files\n");
		if(f1){fclose(f1);}
		if(f2){fclose(f2);}
		return 1;
	}
	while(((read1 = getline(&line1, &len1, f1)) != -1) && 
		  ((read2 = getline(&line2, &len2, f2)) != -1)){
			if(strcmp(line1, line2)){
				ans = 1;
				break;
			}
		  }
	if(read2 < 0 || getline(&line2, &len2, f2) != -1)
		ans = 1; 
	fclose(f1);
	fclose(f2);
	if(line1){free(line1);}
	if(line2){free(line2);}
	return ans;
}
