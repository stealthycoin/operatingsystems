#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slug_mem.h"

/* Test program to demonstrate forgotten deallocation
 */

int main(void){
    char *string[4];
    int i, *num[4];
    const char* hello = "Hello, world!\n";

    printf("Test program to demonstrate forgotten deallocation.\n");
    printf("\n\n");
    for (i = 0; i < 4; i++) {
        string[i] = (char *) malloc(15*sizeof(char));
        num[i] = (int *) malloc(1*sizeof(int));
        strcpy(string[i], hello);
        *num[i] = i;
    }

    for (i = 0; i < 4; i++) {
        free(string[i]);
    }

    return 0;
}
