#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

#define BUFFER_SIZE 1024
#define TIMELIMIT 5000
int id = 1;
int fd = 0;

typedef struct cell_s {
        int length;
        off_t offset;
} cell_s;

int buildOffsetTable(cell_s *arr) {
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

void printAll(cell_s *arr){
        for(int lineNum = 1; lineNum <= id; lineNum++){
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
        }
}

int readLineNumLimit(cell_s *arr) {
        int pollReturn;
        struct pollfd fds;
        fds.fd = STDIN_FILENO;
        fds.events = POLLIN;
        while(1) {
                printf("Enter line number until %lf seconds runs out : ", (double)TIMELIMIT / 1000);
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
                        printAll(arr);
                        if (close(fd)) {
                                perror("Closing file failed");
                                return -1;
                        }
                        exit(1);
                }
                return readLineNum();
        }
}

void printLine(int lineNum, cell_s *arr) {
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

        cur_len = buildOffsetTable(arr);
        while ((lineNum = readLineNumLimit(arr)) != 0) {
                if (lineNum == -1) {
                        continue;
                }
                printf("\n");
                printLine(lineNum, arr);
        }

        if (close(fd)) {
                perror("Closing file failed");
                return -1;
        }
        return 0;
}
