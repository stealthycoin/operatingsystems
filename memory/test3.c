#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slug_mem.h"

/* Test program to demonstrate invalid deallocation
 */

int main(void){
    char *string;
    int *num;
    const char* hello = "Hello, world!\n";
    string = (char *) malloc(15*sizeof(char));
    strcpy(string, hello);

    free(string);
    free(num);

    return 1;
}
