#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slug_mem.h"

/* Test program to demo proper allocation and deallocation
 */

int main(void){
    char *string1, *string2;
    string1 = (char *) malloc(sizeof(char) * 8);
    string2 = (char *) malloc(sizeof(char) * 6);
    const char* test1 = "Testing";
    const char* test2 = "1 2 3";
    strcpy(string1, test1);
    strcpy(string2, test2);
    printf("%s %s\n", string1, string2);

    free(string1);
    free(string2);
    
    return 0;
}
