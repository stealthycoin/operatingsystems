/* CREATED 1 May 2014
   Team Brilliant Squid
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

  int k = 4000000000;
  int s;
  int i;
  int j;
  if(argc < 2){
    printf("Usage: cpu_bound <seconds>\n");
    exit(1);
  }
  
  s = atoi(argv[1]);
  i = 0;
  j = 10;
  for(i = 0; i < k * s; i++){
    j = j * 3; 
  }
  
  return 0;
}
