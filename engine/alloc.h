#ifndef ALLOC_H
#define ALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include "util/log.c"

void * alloc_make(size_t size) {
  log_debug("alloc_make: request: size=%d", size);
  void *ptr = malloc(size);
  if (ptr == NULL) {
    log_error("alloc_make: failed to allocate: size=%zu", size);
    exit(1);
  }
  log_debug("alloc_make: success: ptr=%p", ptr);
  return ptr;
}

void * alloc_resize(void *ptr, size_t size) {
  log_debug("alloc_resize: request: size=%d ptr=%p", size, ptr);
  ptr = realloc(ptr, size);
  if (ptr == NULL) {
    log_error("alloc_resize: failed to allocate: size=%zu", size);
    exit(1);
  }
  log_debug("alloc_resize: success: size=%d ptr=%p", size, ptr);
  return ptr;
}

void * alloc_free(void *ptr) {
  log_debug("alloc_free: ptr=%p", ptr);
  free(ptr);
  return NULL;
}

void *alloc_push(void *ptr, int *size, int *cap, const void *elem, size_t elem_size) {
  log_debug("alloc_push: request: ptr=%p size=%d cap=%d elem_size=%d elem=%p", ptr, *size, *cap, elem_size, elem);
  if (*size == *cap) {
    if (*cap == 0) {
      *cap = 16;
    } else {
      *cap *= 2;
    }
    ptr = alloc_resize(ptr, *cap * elem_size);
  }
  void *dest = (char*)ptr + *size * elem_size;
  memcpy(dest, elem, elem_size);
  *size += 1;
  log_debug("alloc_push: success: ptr=%p size=%d cap=%d elem_size=%d elem=%p", ptr, *size, *cap, elem_size, elem);
  return ptr;
}

#endif