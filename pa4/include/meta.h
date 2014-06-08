/* CREATED -- JUNE 8th 2014 */
#include <lib.h>
#include <unistd.h>

/* This function is a harness which calls our syscall do_metar with the appropriate message */
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

/* This function is a harness which calls our syscall do_metaw with the appropriate message */
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
