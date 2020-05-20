#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int status;
	int forkRet;

	if((forkRet = fork()) == 0) {
		execvp(argv[1], &argv[1]);
		perror(argv[1]);
		exit(1);
	}
	else if(forkRet == -1) {
		perror("Creating a new process was failed!");
		exit(1);
	}
	else {
		wait(&status);
		if(WIFSIGNALED(status)) {
			printf("Child process terminated due to the signal\n");
			exit(1);
		}
		if(WIFEXITED(status)) {
			printf("Child process finished with status: %d\n", WEXITSTATUS(status));
			printf("I am parent process!\n");
			exit(1);
		}		
	}

	return 0;
}
