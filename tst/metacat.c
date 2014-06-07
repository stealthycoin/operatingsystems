#include <meta.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (int args, char **argv) {
  int err, fd, nbytes;
  char *fname, *buffer;

  if (args < 2) {
    printf ("Too few arguments.\nUsage: metacat FILE\n");
    return 1;
  }

  fname = argv[1];
  if ((fd = open (fname, O_WRONLY | O_APPEND)) == -1) {
    printf ("File didn't open.\n");
    return 1;
  }
  nbytes = 1024;

  err = metar (fd, buffer, nbytes);
  printf ("%s\n", buffer);
  return err;
}
