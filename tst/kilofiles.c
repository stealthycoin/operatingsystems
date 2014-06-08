#include <stdio.h>
#include <meta.h>
#include <fnctl.h>
#include <string.h>
#include <unistd.h>


int main (void)
{
    int i, fd[1000];
    char name[6], junk[5] = "junk";


    for (i = 0; i < 1000; i++) {
        sprintf(name, "file%d", i);
        fd[i] = open(name, O_WRONLY | O_CREATE);
        metaw(fd[i], junk, 5);
    }

    return 0;
}
