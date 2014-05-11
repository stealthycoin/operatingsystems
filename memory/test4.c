#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slug_mem.h"

/* Test program to demonstrate an attempt to deallocate
 * previously deallocated memory.
 */

int main(void){
    char *string;
    const char* hello = "Hello, world!\n";
    string = (char *) malloc(15*sizeof(char));
    strcpy(string, hello);
    free(string);
    free(string);

    return 1;
}
