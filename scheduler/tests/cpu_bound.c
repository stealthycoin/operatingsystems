#include <stdio.h>
#include <math.h>

int main(void)
{
  int x;
  float y, z;

  for (x = 0; x < 2560000; x++) {
    y = sin(x);
    z += y;
  }

  printf("%f\n", z);
}
