#ifndef DEBUG_H
#define DEBUG_H

#include "unity.h"


void print_mat4(glm::mat4 mat) {
  const float* result = (const float*)glm::value_ptr(mat);

  printf("%10.2f %10.2f %10.2f %10.2f\n%10.2f %10.2f %10.2f %10.2f\n%10.2f "
         "%10.2f %10.2f %10.2f\n%10.2f %10.2f %10.2f %10.2f\n---\n",
         result[0], result[1], result[2], result[3], result[4], result[5],
         result[6], result[7], result[8], result[9], result[10], result[11],
         result[12], result[13], result[14], result[15]);
}
#endif