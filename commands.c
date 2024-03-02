//		commands.c
//********************************************
#include "commands.h"
char *last_path, *tmp;
// for signal handlers
bool is_running = false;
char* running_cmd;
int running_pid;
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString)
{
	sweep_zombies();
	int bg = 0;
	if (lineSize[strlen(lineSize)-2] == '&'){
		lineSize[strlen(lineSize)-2] = '\0';
		bg = 1;
	}
	char* cmd; 
	char* args[MAX_ARG];
	//char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
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
		if(num_arg != 1){
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
				if(chdir(args[1])){PRINT_SYS_ERROR(chdir);}
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
 		jobs_print();
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		printf("smash pid is %d\n", getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		int ret, job_id, stat;
		int valid = 1;
		char job_cmd[MAX_LINE_SIZE];
		if(args[1] != NULL){
			job_id = atoi(args[1]);
			if(!job_id)
				valid = 0;
		}	
		else
			job_id = 0;
		if(num_arg > 1 || !valid)
			fprintf(stderr, "smash error: fg: invalid arguments\n");
		else{
			ret = fg_run(job_id, job_cmd);
			switch(ret){
				case no_jobs:
					fprintf(stderr, "smash error: fg: jobs list is empty\n");
					break;
				case job_not_found:
					fprintf(stderr, "smash error: fg: job-id %d does not exist\n", job_id);
					break;
				default: // bring to front
					running_pid = ret;
					running_cmd = job_cmd;
					is_running = true;
					waitpid(ret, &stat, 0);
					is_running = false;
					break;
			}
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		int ret, job_id;
		int valid = 1;
		if(args[1] != NULL){
			job_id = atoi(args[1]);
			if(!job_id)
				valid = 0;
		}	
		else
			job_id = 0;
		if(num_arg > 1 || !valid)
			fprintf(stderr, "smash error: bg: invalid arguments\n");
		else{
			ret = bg_run(job_id);
			switch(ret){
				case no_stopped_jobs:
					fprintf(stderr, "smash error: bg: there are no stopped jobs to resume\n");
					break;
				case wrong_type:
					fprintf(stderr, "smash error: bg: job-id %d is already running in the background\n", job_id);
					break;
				case job_not_found:
					fprintf(stderr, "smash error: bg: job-id %d does not exist\n", job_id);
					break;
				default:
					break;
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		if(num_arg == 1 && !strcmp(args[1], "kill"))
			kill_jobs();
		exit(0);
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill")){
		int ret, job_id, sig_num;
		job_id = atoi(args[2]);
		sig_num = atoi(args[1]);
		sig_num = (sig_num < 0) ? -sig_num : 0;
   		if(num_arg != 2 || !job_id || !sig_num){
			fprintf(stderr, "smash error: kill: invalid arguments\n");
		}
		else{
			if((ret = sig_job(job_id, sig_num))){
				if(ret == -1)
					fprintf(stderr, "smash error: kill: job_id %d does not exist\n", job_id);
				else
					printf("signal number %d was sent to pid %d\n", sig_num, job_id);
			}
		}
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
		//check if background
		// printf("line: %s\n", cmdString);
 		ExeExternal(args, cmdString, bg);
	 	return 0;
	}
	if (illegal_cmd == true)
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
void ExeExternal(char *args[MAX_ARG], char* cmdString, int bg)
{
	int pid, ret;
    switch(pid = fork()) 
	{
    	case -1: 
			PRINT_SYS_ERROR(fork);
        case 0 :
            // Child Process
            setpgid(0, 0);
			execv(args[0], args);

			PRINT_SYS_ERROR(execv);
			exit(-1);
		default:
			if(!bg){ // wait if normal cmd
				running_pid = pid;
				running_cmd = cmdString;
				is_running = true;
				if(waitpid(pid, &ret, 0) == -1 && errno != EINTR){
					PRINT_SYS_ERROR(waitpid);
					printf("wtf?");
				}		
				is_running = false;
			}
			else{
				jobs_add(background, pid, cmdString);	
			}
			return;
	}
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************

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

bool IsRunning(){
	return is_running;
}

int RunningPID(){
	return running_pid;
}

char* RunningCMD(){
	return running_cmd;
}