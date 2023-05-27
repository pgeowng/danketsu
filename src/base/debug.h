#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H

#include <stdio.h>

#define ASSERT(x)                                                              \
  if (!(x)) {                                                                  \
    printf("%s:%d ASSERTION FAILED: %s",__FILE__, __LINE__, #x);                                        \
    __debugbreak();                                                            \
  }

#define FAIL(x)                                                              \
  if (1) {                                                                  \
    printf("%s:%d ASSERTION FAILED: %s",__FILE__, __LINE__, #x);                                        \
    __debugbreak();                                                            \
  }
#endif