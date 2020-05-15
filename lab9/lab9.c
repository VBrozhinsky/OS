#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
	int status;
	char *argv[] = { "cat", "lab9.c", '\0' };

	if(fork() == 0) {
		if(execvp(argv[0], argv) == -1)
			perror(argv[0]);
		exit(222);
	}
	else {
		wait(&status);
		printf("Child process finished with status: %d\n", WEXITSTATUS(status));
		printf("I am parent process!\n");
	}

	return 0;
}
