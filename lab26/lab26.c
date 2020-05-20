#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

#define BUFFERSIZE 66

void print_upper_string(const char *string, FILE *out) {
	char line[BUFFERSIZE];
	int i = 0;

	while (string[i] != NULL) {
		line[i] = toupper(string[i]);
		i++;
	}
	
	fputs(line, out);
}

int main() {
	FILE *in, *out;
	static char buf[BUFFERSIZE];
	static int status1, status2;
	
	if (((in = popen("cat text.txt", "r")) == NULL) || ((out = popen("cat", "w")) == NULL)) {
		perror("Cannot create child process");
		exit(-1);
	}
	else {
		while(fgets(buf ,BUFFERSIZE, in) != (char *)NULL) {
			print_upper_string(buf, out);
		}		
	}
	
	status1 = pclose(in);
	status2 = pclose(out);

	if(WIFEXITED(status1)) {
		printf("Child process 1 finished with status: %d\n", WEXITSTATUS(status1));
	}
	if(WIFEXITED(status2)) {
		printf("Child process 2 finished with status: %d\n", WEXITSTATUS(status2));
	}
		 	
	return 0;
}
