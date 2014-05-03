/* CREATED 1 May 2014
   Team Brilliant Squid
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

  long k = 100000;
  long s;
  long i;
  long j;
  if(argc < 2){
    printf("Usage: cpu_bound <seconds>\n");
    exit(1);
  }
  
  s = atoi(argv[1]);
  i = 0;
  j = 10;
  for(i = 0; i < k * s; i++){
    int l = 0; 
    for(l = 0; l < 10000; l++){
      j = j * 3; 
      j = 20; 
    }
  }
  
  return 0;
}
