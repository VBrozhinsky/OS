#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#define TEXTLENGTH 66

void print_upper_string(const char *string, int length) {
	char ch;
	for(int i = 0; i < length; i++) {
		ch = string[i];
		putchar(toupper(ch));
	}
} 

int main() {
	int fd[2];
	pid_t pid_1, pid_2;
	static char textout[TEXTLENGTH] = "The Matrix has you...\n Follow the white rabbit...\n";
	static char textin[TEXTLENGTH];
	int status_1, status_2;
	int length;
	
    if (pipe(fd) == -1) {
        perror("Pipe");
        exit(-1);
    }

    if ((pid_1 = fork()) == 0) {
        write(fd[0], textout, TEXTLENGTH);
        exit(1);
    }
    else if (pid_1 == -1) {
        perror("Cannot create child process");
        exit(-1);
    }

    if (pid_1 > 0 && (pid_2 = fork()) == 0) {
        length = read(fd[1], textin, TEXTLENGTH);
        print_upper_string(textin, length);
        exit(2);
    }
    else if (pid_2 == -1) {
        perror("Cannot create child process");
        exit(-1);
    }


	if(WIFSIGNALED(status_1)) {
        printf("Child 1 terminated due to the signal\n");
    }
    if(WIFEXITED(status_1)) {
        printf("Child 1 finished with status: %d\n", WEXITSTATUS(status_1));
    }

    if(WIFSIGNALED(status_2)) {
        printf("Child 2 terminated due to the signal\n");
    }
    if(WIFEXITED(status_2)) {
        printf("Child 2 finished with status: %d\n", WEXITSTATUS(status_2));
    }

    if(close(fd[0])) {
        perror("Closing pipe failed");
        return -1;
    }
    if(close(fd[1])) {
        perror("Closing pipe failed");
        return -1;
    }
    
    return 0;
}

