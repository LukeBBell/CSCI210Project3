#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for (int i = 0; i< N; i++) {
		if(strcmp(allowed[i],cmd) == 0) {
			return 1;
		}
	}
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char *argv[20];
    char* cmd;
    int argc;
    int status;
    pid_t pid;
    posix_spawnattr_t attr;
    posix_spawnattr_init(&attr);
    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c
	char* token = strtok(line, " ");
	argc = 0;
	cmd = token;
	token = strtok(NULL, " ");
	while (token != NULL) {
		argc++;
		argv[argc] = token;
		token = strtok(NULL, " ");
	}
	argv[argc + 1] = NULL;
	if (isAllowed(cmd) == 1) {
		if (strcmp(cmd,"cd") == 0) {
			if (argc > 1) {
				printf("-rsh: cd: too many arguments\n");
				continue;
			}
			chdir(argv[1]);
		} else if (strcmp(cmd,"exit") == 0) {
			break;
		} else if (strcmp(cmd,"help") == 0) {
			printf("The allowed commands are:\n1: cp\n2: touch\n3:mkdir\n4:ls\n5: pwd\n6: cat\n7: grep\n8:chmod\n9: diff\n10: cd\n11: exit\n12:help");
		} else {
			if (posix_spawnp(&pid,cmd, NULL, &attr, argv, environ) != 0) {
				perror("spawn failed");
				exit(EXIT_FAILURE);
			}
			if (waitpid(pid,&status, 0) == -1) {
				perror("waitpid failed");
				exit(EXIT_FAILURE);
			}
		}	
	} else {
		printf("NOT ALLOWED!");
	}
    }
    return 0;
}
