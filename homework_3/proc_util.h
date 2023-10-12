#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include "file_util.h"
#define MAX_PIDS 1000
#define NAME_OF_ENTRY entry -> d_name
#define MAX_PATH_LENGTH 1000
#define FOR_EACH(iter, arr, length) \
	for(int i = 0; i < length; ++i){ iter = arr[i];
#define END_FOR_EACH }
void get_pids(int *pids, int* length){
	struct dirent *entry;
	DIR *dp = opendir("/proc");
	if(dp == NULL){
		perror("Failed to open /proc");
		return;
	}
	int counter = 0;
	while((entry = readdir(dp)) && counter < MAX_PIDS){
		if(!isdigit(NAME_OF_ENTRY[0]))
			continue;
		int pid = atoi(NAME_OF_ENTRY);
		pids[counter] = pid;
		counter ++;
	}
	closedir(dp);
	*length = counter;
}

void read_cmdline(char* path, char* buffer){
	FILE *cmdfile = fopen(path, "r");
	if(!cmdfile)
		return;
	size_t bytes_to_read = fread(buffer, 1, MAX_PATH_LENGTH, cmdfile);
	fclose(cmdfile);
	buffer[bytes_to_read] = '\0';
}

void read_pid(int* pid, char* result){
	char path[MAX_PATH_LENGTH];
	snprintf(path, sizeof(path), "/proc/%d/cmdline", *pid);
	read_cmdline(path, result);
}
void print_process(int* pid){
	char result[MAX_PATH_LENGTH];
	read_pid(pid, result);
	printf("Process ID: %d Command: %s\n", *pid, result);
}

void print_processes(){
	int pid, length;
	int pids[MAX_PIDS];
	get_pids(pids, &length);
	FOR_EACH(pid, pids, length)
		print_process(&pid);
	END_FOR_EACH
}

void search_pid(int* pid, char* cmd){
	int ipid, length;
	int pids[MAX_PIDS];
	get_pids(pids, &length);
	FOR_EACH(ipid, pids, length)
		char name[MAX_PATH_LENGTH];
		read_pid(&ipid, name);
		if(!strcmp(name, cmd) || *pid == ipid)
			break;
	END_FOR_EACH
	*pid = ipid;
}
int kill_proc(int pid, char* cmd){
	search_pid(&pid, cmd);
	if(!kill(pid, SIGKILL))
		return 0;
	perror("Failed to kill process");
	return 1;
}

