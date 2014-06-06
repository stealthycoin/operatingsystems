#include <lib.h>
#include <unistd.h>

PUBLIC int metar (fd, buffer, nbytes)
int fd;
void *buffer;
int nbytes;
{
  message m;
  m.m1_i1 = fd;
  m.m1_i2 = nbytes;
  m.m1_p1 = (char *) buffer;

  return ( _syscall(VFS_PROC_NR, METAR, &m) );
}

PUBLIC int metaw (fd, buffer, nbytes)
int fd;
void *buffer;
int nbytes;
{
  message m;
  m.m1_i1 = fd;
  m.m1_i2 = nbytes;
  m.m1_p1 = (char *) buffer;

  return ( _syscall(VFS_PROC_NR, METAW, &m) );
}
