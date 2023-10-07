#include "stdint.h"
#include "stdio.h"

int main() {
  uint32_t u = 123;
  float f = (float)u;

  printf("%u %f\n", u, f);

  return 0;
}