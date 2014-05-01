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
    printf("Usage: IO_bound <filename>\n");
    exit(1);
  }
  
  char *fname = argv[1];
  FILE *f = fopen(fname, "rw");
  FILE *f2 = fopen("IO_bound.out", "w");
  if(!f || !f2){
    printf("Failed to open file %s\n", argv[1]);
    exit(1);
  }
  char buf[1025];
  
  int sum = 0; 
  int i = 0; 
  size_t n = fread(buf, sizeof(char), 1024, f); 
  while(n > 0){
    int w = fwrite(buf, sizeof(char), n, f2);
    if(!w){ 
      printf("Failed to write to file IO_bound.out\n");
      exit(1);
    }
    for(i = 0; i < n; i++){
      sum += buf[i]; 
    }
    n = fread(buf, sizeof(char), 1024, f); 
    
  }
  printf("Checksum: %d\n", sum); 
  fclose(f);
  
  
  return 0;
}
