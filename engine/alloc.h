#ifndef ALLOC_H
#define ALLOC_H

#include <stdio.h>
#include <stdlib.h>

void * alloc_make(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    printf("Error: malloc failed to allocate %zu bytes\n", size);
    exit(1);
  }
  return ptr;
}

void * alloc_resize(void *ptr, size_t size) {
  ptr = realloc(ptr, size);
  if (ptr == NULL) {
    printf("Error: realloc failed to allocate %zu bytes\n", size);
    exit(1);
  }
  return ptr;
}

void * alloc_free(void *ptr) {
  free(ptr);
  return NULL;
}

#endif