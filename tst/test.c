#include <meta.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (void)
{
  int err;
  int fd = open("do.sh", O_WRONLY | O_APPEND);
  char *buffer = "Hello World";
  char out[1025];
  int nbytes = 12;
  err = metaw(fd, buffer, nbytes);
  nbytes = 1024;
  err = metar(fd, out, nbytes);
  printf ("%s\n", out);
  return 0;
}
