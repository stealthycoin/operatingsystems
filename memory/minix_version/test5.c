#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slug_mem.h"

/* Test program to alloc memory and then exit
 * 
 */

int main(void){
    char *string;
    const char* hello = "Hello, world!\n";

    printf("Test program to alloc memory and then exit.\n");
    printf("\n\n");
    string = (char *) malloc(15*sizeof(char));

    return 0;
}
