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
	int len = sizeof(allowed);
	for (int i = 0; i<= len; i++) {
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
    char *argv[21];
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
	while (token != NULL && argv < 20) {
		argv[argc++] = token;
		token = strtok(NULL, " ");
	}
	argv[argc] == NULL;
	if (argc == 0) continue;
	if (isAllowed(line) == 1) {
		if (strcmp(line,"cd") == 0) {
			chdir(argv[1]);
		} else if (strcmp(line,"exit") == 0) {
			exit(0);
		} else if (strcmp(line,"help") == 0) {
			printf("1: cp\n2: touch\n3:mkdir\n4:ls\n5: pwd\n6: cat\n7: grep\n8:chmod\n9: diff\n10: cd\n11: exit\n12:help");
		} else {
			pid_t pid;
			int status;
			posix_spawnattr_t attr;
			posix_spawnattr_init(&attr);
			if (posix_spawnp(&pid,argv[0], NULL, &attr, argv, environ) != 0) {
				perror("spawn failed");
				exit(EXIT_FAILURE);
			}
			if (waitpid(pid,&status, 0) == -1) {
				perror("waitpid failed");
				exit(EXIT_FAILURE);
			}
			if (WIFEXITED(status)) {
				printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
			}
			posix_spawnattr_destroy(&attr);
		}	
	} else {
		printf("NOT ALLOWED!");
	}
    }
    return 0;
}
