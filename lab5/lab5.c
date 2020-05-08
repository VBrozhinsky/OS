#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1024
int id = 1;
int fd = 0;

typedef struct cell_s {
	int length;
	off_t offset;
} cell_s;

int buildOffsetTable(cell_s *arr, int fd) {
	char buf[2];
	int cur_len = 0;
	arr[id].offset = 0;
	while(read(fd, buf, 1) != 0) {
		if (errno == EINTR){
			continue;
		}
		cur_len++;
		if(buf[0] == '\n') {
			arr[id].length = cur_len;
			arr[id + 1].offset = lseek(fd, 0L, SEEK_CUR);
			id++;
			cur_len = 0;
		}
	}
	return cur_len;
}

int readLineNum() {
	int lineNum = -1;
	char term;
	printf("Enter line number: ");
	
	if(scanf("%d%c", &lineNum, &term) != 2 || term != '\n') {
		while (errno == EINTR) {
			scanf("%d%c", &lineNum, &term);
		}
		printf("Integer value required!\n");
		if (close(fd)) {
			perror("Closing file failed");
			return -1;
		}
		exit(1);
	}
	return lineNum;
}

void printLine(int fd, int lineNum, cell_s *arr) {
	char buf[BUFFER_SIZE];
	if(lineNum != 0 && lineNum < id) {
		lseek(fd, arr[lineNum].offset, 0);
		read(fd, buf, arr[lineNum].length - 1);
		if (errno == EINTR){
			read(fd, buf, arr[lineNum].length - 1);
		}
		printf("%s\n", buf);
		memset(buf, 0, BUFFER_SIZE);
	}

	if(lineNum >= id)
		printf("Line number should be <= %d\n", id);
}

int main() {
	cell_s arr[BUFFER_SIZE];
	int fd = 0;
	int cur_len;
	int lineNum = 0;
	
	if((fd = open("test_file.txt", O_RDONLY)) == -1) {
		perror("Cannot open file");
		perror(strerror(errno));
		if (close(fd)) {
			perror("Closing file failed");
			return -1;
		}
		exit(1);
	}
	
	cur_len = buildOffsetTable(arr, fd);
	while ((lineNum = readLineNum()) != 0) {
		if (lineNum == -1) {
			continue;
		}
		printLine(fd, lineNum, arr);
	}
	
	if (close(fd)) {
		perror("Closing file failed");
		return -1;
	}
	return 0;
}
