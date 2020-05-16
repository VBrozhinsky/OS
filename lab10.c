#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int status;

	if(fork() == 0) {
		execvp(argv[1], &argv[1]){
		perror(argv[1]);
		exit(222);
	}
	else {
		wait(&status);
		проверить есть ли этот статус(процесс может завершиться не по exit,сигнал WTERMSIG)
		printf("Child process exit status: %d\n", WEXITSTATUS(status));
	}

	return 0;
}
