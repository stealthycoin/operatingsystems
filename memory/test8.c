#include <stdlib.h>
#include <stdio.h>
#include "slug_mem.h"

int main() {

  void *ptr = malloc(1024*1024*129);
  free(ptr);

  return 0;
}
