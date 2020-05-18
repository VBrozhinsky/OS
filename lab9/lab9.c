#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <wait.h>

int main() {
        int status;
        siginfo_t status1;
        int forkRet;
        char *argv[] = { "cat", "lab9.c", '\0' };

        if((forkRet = fork()) == 0) {
                execvp(argv[0], argv);
                perror(argv[0]);
                exit(1);
        }
        else if(forkRet == -1) {
                perror("Creating a new process was failed!");
                exit(1);
        }
        else {
                while(1) {
                        waitid(P_ALL, 0, &status1, WNOWAIT);
                        if(WIFSIGNALED(status)) {
                                printf("child process terminated due to the signal receipt of a signal\n");
                                system("ps -eo pid,user,command,stat");
                                exit(1);
                        }
                        if(WIFEXITED(status)) {
                                printf("Child process finished with status: %d\n", WEXITSTATUS(status));
                                printf("I am parent process!\n");
                                system("ps -eo pid,user,command,stat");
                                exit(1);
                        }
                }
        }

        return 0;
}

