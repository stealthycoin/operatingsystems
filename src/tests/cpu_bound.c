/* CREATED 1 May 2014
   Team Brilliant Squid
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

int getpid(void);

int main(int argc, char **argv)
{

  int k = 100000;
  int s;
  int i;
  int j;
  int pid;
  if(argc < 2){
    printf("Usage: cpu_bound <seconds>\n");
    exit(1);
  }
  
  s = atoi(argv[1]);
  i = 0;
  j = 10;
  pid = getpid();
  printf("Starting cpu_bound, pid=%d\n", pid);
  for(i = 0; i < k * s; i++){
    atan2((double) i,(double) i+1);
  }
  printf("Exiting cpu_bound, pid=%d\n", pid);
  return 0;
}
