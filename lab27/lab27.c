#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BUFFERSIZE 66

int main() {
	FILE *in, *out;
	char buf[BUFFERSIZE];
	static int status;
	
	if((in = fopen("text.txt", "r")) == NULL) {
		perror("Cannot open file");
		exit(1);
	}

	if((out = popen("wc -l", "w")) == NULL) {
		perror("Cannot create child process");
		exit(-1);
	}
	else {
		while(fgets(buf, BUFFERSIZE, in) != (char *)NULL) {
			if(strcmp(buf, "\n") == 0) {
				fputs(buf, out);
			}
		}
	}
	
	if((fclose(in) == -1) {
		perror("Cannot close file");
		exit(1);
	}
	
	status = pclose(out);
	if(WIFEXITED(status)) {
		printf("Child process 2 finished with status: %d\n", WEXITSTATUS(status));
	}

	return 0;
}
