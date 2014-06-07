#include <meta.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (int args, char **argv) {
  int err, fd, nbytes;
  char *fname, *tag;

  if (args < 3) {
    printf ("Too few arguments.\nUsage: metatag FILE TEXT\n");
    return 1;
  }

  fname = argv[1];
  tag = argv[2];
  if ((fd = open (fname, O_WRONLY | O_APPEND)) == -1) {
    printf ("File didn't open.\n");
    return 1;
  }
  nbytes = strlen (tag);

  err = metaw (fd, tag, nbytes);
  return err;
}
