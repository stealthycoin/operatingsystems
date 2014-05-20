#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slug_mem.h"

/* Test program to alloc memory and then exit
 * 
 */

int main(void){
    char *string;
    int i = 0;
    for (i = 0; i < 100; i++) {
      const char* hello = "Hello, world!\n";
      string = (char *) malloc(15*sizeof(char));
    }
    free(string);

    return 0;
}
