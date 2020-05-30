#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFERSIZE 66

int main() {
        FILE *in;
        char buf[BUFFERSIZE];
        static int status;

        if((in = popen("cat text.txt | grep '^$' | wc -l", "r")) == NULL) {
                perror("Cannot create child process");
                exit(-1);
        }

        fgets(buf, BUFFERSIZE, in);

        status = pclose(in);
        if(status == -1){
                perror(strerror(errno));
        }
        else if(WIFEXITED(status)) {
                printf("Child process finished with status: %d\n", WEXITSTATUS(status));
                if(status == 0){
                        printf("%s", buf);
                }
        }

        return 0;
}

