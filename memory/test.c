#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FUNCTIONIZE(a,b) a(b)
#define STRINGIZE(a)     #a
#define INT2STRING(i)    FUNCTIONIZE(STRINGIZE, i)
#define FILE_POS         __FILE__ ":" INT2STRING(__LINE__)

void *slug_malloc(size_t size, char *WHERE);

int main(void){
  char *string1, *string2;
  string1 = (char *) slug_malloc(sizeof(char) * 10, FILE_POS);
  string2 = (char *) slug_malloc(sizeof(char) * 10, FILE_POS);
  const char* test1 = "Testing";
  const char* test2 = "1 2 3";
  strcpy(string1, test1);
  strcpy(string2, test2);
  printf("%s %s\n", string1, string2);
}
