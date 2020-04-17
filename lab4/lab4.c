#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct node_s {
        char* str;
        struct node_s* next;
} node_s;

void clear(node_s** head){
	node_s* c;
	while(head){
		c = (*head);
		(*head) = (*head)->next;
		free(c);
	}
}

void push(node_s** head, const char* input) {
        node_s* tmp = malloc(sizeof(node_s));
        if (tmp) {
                tmp->str = malloc(strlen(input) + 1);
                strcpy(tmp->str, input);
                //memccpy(tmp->str, input, '.', strlen(input) + 1);
        }
        else {
                perror("!!!Pushing: memory allocation!!!");
                clear(head);
                exit(1);
        }
        tmp->next = (*head);
        (*head) = tmp;
}

void pop(node_s** head) {
        node_s* prev = NULL;
        int val;

        prev = (*head);
        (*head) = (*head)->next;

        printf("%s", prev->str);

        free(prev->str);
        free(prev);
}

int main() {
        char input[1024] = { 0 };
        node_s* head = NULL;

        while (input[0] != '.') {
                fgets(input, 1024, stdin);
                push(&head, input);
        }

        while (head)
            pop(&head);

        return 0;
}

