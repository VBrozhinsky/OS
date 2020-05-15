#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <sys/mman.h>

#define BUFFER_SIZE 1024
#define TIMELIMIT 5000
int id = 1;
int fd = 0;

typedef struct cell_s {
    int length;
    off_t offset;
} cell_s;

int buildOffsetTable(char* fileMap, off_t fileMapSize, cell_s *arr) {
    char buf[2];
    int cur_len = 0;
    arr[id].offset = 0;
    while(1) {
        if (id == fileMapSize || cur_len >= BUFFER_SIZE) {
			break;
		}
        cur_len++;
        if(fileMap[id] == '\n') {
            arr[id].length = cur_len;
            arr[id + 1].offset = id;
            id++;
            cur_len = 0;
        }
    }
    return cur_len;
}

int readLineNum() {
    int lineNum = -1;
    char term;
    //printf("Enter line number: ");
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

void printAll(char* fileMap, off_t fileMapSize, cell_s *arr){
	int length;
	int startPos;
    for(int lineNum = 1; lineNum <= id; lineNum++){
        length = arr[lineNum].offset; - arr[lineNum - 1].offset;;
		startPos = 1 + arr[lineNum - 1].offset;
	
		write(STDOUT_FILENO, fileMap + startPos, sizeof(char) * length);
		printf("\n");
    }
}

int readLineNumLimit(char* fileMap, off_t fileMapSize, cell_s *arr) {
    int pollReturn;
    struct pollfd fds;
    fds.fd = STDIN_FILENO;
    fds.events = POLLIN;
    while(1) {
        pollReturn = poll(&fds, 1, TIMELIMIT);
        if (errno == EINTR) {
            continue;
        }
        else if (pollReturn == -1) {
            perror(strerror(errno));
            return -1;
        }
        else if (pollReturn == 0) {
        	printf("\nTime is up!\n");
            printAll(fileMap, fileMapSize, arr);
            if (close(fd)) {
                perror("Closing file failed");
                return -1;
            }
            exit(1);
        }
        return readLineNum();
    }
}

void printLine(char* fileMap, off_t fileMapSize, int lineNum, cell_s *arr) {
    /*char buf[BUFFER_SIZE];
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
    printf("Line number should be <= %d\n", id);*/
    
    int length = arr[lineNum].offset; - arr[lineNum - 1].offset;;
	int startPos = 1 + arr[lineNum - 1].offset;
	
	write(STDOUT_FILENO, fileMap + startPos, sizeof(char) * length);
	printf("\n");
}

int main() {
    cell_s arr[BUFFER_SIZE];
    int cur_len;
    int lineNum = 0;
    char *fileMap = NULL;
	off_t fileMapSize;

    if((fd = open("test_file.txt", O_RDONLY)) == -1) {
        perror("Cannot open file");
        perror(strerror(errno));
        if (close(fd)) {
        	perror("Closing file failed");
            return -1;
        }
        exit(1);
    }
    
    fileMapSize = lseek(fd, 0, SEEK_END);
	fileMap = (char*) mmap(0, fileMapSize, PROT_READ, MAP_SHARED, fd, 0);
	if (fileMap == NULL) {
		perror(strerror(errno));
		if (close(fd)) {
        	perror("Closing file failed");
            return -1;
        }
        exit(1);
	}

    cur_len = buildOffsetTable(fileMap, fileMapSize, arr);
    while (1) {
    	printf("Enter line number until %lf seconds runs out : \n", (double)TIMELIMIT / 1000);
		if((lineNum = readLineNumLimit(fileMap, fileMapSize, arr)) == 0) {
			break;
		}
        if (lineNum == -1) {
            continue;
    	}
        printLine(fileMap, fileMapSize, lineNum, arr);
    }

	if (munmap(fileMap, fileMapSize) == -1) {
		perror(strerror(errno));
		puts("Unable to unmap the file");
	}
	
    if (close(fd)) {
        perror("Closing file failed");
        return -1;
    }
    return 0;
}
