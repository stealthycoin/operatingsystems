#include <stdlib.h>
#include <stdio.h>
#include "slug_mem.h"

struct list_el {
    int *val;
    struct list_el *next;
};

typedef struct list_el item;

int main(void) {
    item *curr, *head;
    int i;

    printf("A test program to allocate a linked list and exit.\n");
    printf("\n\n");
    head = NULL;

    for(i = 0; i < 10; i++) {
        curr = (item *)malloc(sizeof(item));
        curr->val = (int*)malloc(sizeof(int)*10);
        curr->next  = head;
        head = curr;
    }

    curr = head;

    while(curr) {
        printf("%p\n", curr->val);
        curr = curr->next ;
    }

    return 0;
}
