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
	for(int i = 0; i < N; i++){
		if(strcmp(cmd, allowed[i]) == 0){
			return 1;
		}	
	}
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    int argc;
    char *argv[21];
    pid_t pid;
    int status;
    posix_spawnattr_t attr;
    posix_spawnattr_init(&attr);

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';
	
	argc = 0;
    	argv[argc] = strtok(line, " ");
   	while (argv[argc] != NULL && argc < 20) {
        	argc++;
        	argv[argc] = strtok(NULL, " ");
	}
	argv[argc] = NULL;  // Null-terminate argv

        // Skip empty input
        if (argc == 0) continue;
	
	if (!isAllowed(argv[0])) {
            fprintf(stderr, "NOT ALLOWED!\n");
            continue;
        }

	if (strcmp(argv[0], "cd") == 0) {
            if (argc != 2) {
                fprintf(stderr, "-rsh: cd: too many arguments\n");
            } 
	    else if (chdir(argv[1]) != 0) {
                perror("rsh: cd");
            }
        } 
	else if (strcmp(argv[0], "exit") == 0) {
            break;
        } 
	else if (strcmp(argv[0], "help") == 0) {
	    printf("The allowed commands are: \n");
            for (int i = 0; i < N; i++) {
                printf("%d: %s\n",i + 1, allowed[i]);
            }
        } 
	
	else {
            if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
                perror("spawn failed");
            } 
	    else {
                waitpid(pid, &status, 0);  // Wait for the process to finish
            }
        }
    }

    // Clean up spawn attributes
    posix_spawnattr_destroy(&attr);

    return 0;
	// TODO
	// Add code to spawn processes for the first 9 commands

	// And add code to execute cd, exit, help command
	
	// Use the example provided in myspawn.c
	
}
