#include <stdlib.h>
#include <stdio.h>
#include "slug_mem.h"

int main(void) {
    void *ptr;
    
    printf("A test program to allocate too much memory.\n");
    printf("\n\n");
    ptr = malloc(1024*1024*129);
    free(ptr);

    return 0;
}
