#include <sys/resource.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int n,char** s){
  pid_t pid;

  pid = getpid();

  while (1) {sleep(2);  printf("%d:%d\n",pid,getpriority(PRIO_PROCESS,pid));}
  return 0;
}
