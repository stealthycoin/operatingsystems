/* CREATED 1 May 2014
   Team Brilliant Squid
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

  int k = 400000000;
  if(argc < 2){
    printf("Usage: cpu_bound <seconds>\n");
    exit(1);
  }
  
  int s = atoi(argv[1]);
  int i = 0;
  int j = 10;
  for(i = 0; i < k * s; i++){
    j = j * 3; 
  }
  
  return 0;
}
