#ifndef SLICE_H
#define SLICE_H

#include <stdio.h>
#include <stdlib.h>
#include "util/log.c"
#include "alloc.h"

#define Type void *



typedef struct slice_s
{
  int		cap;
  int		size;
  size_t elem_size;
  Type data;
} slice_t;


void slice_make( slice_t *slice, int size, size_t elem_size ) {
  assert( slice != NULL );
  assert( slice->data == NULL );

  slice->data = alloc_make( size * elem_size );
  slice->size = 0;
  slice->cap = size;
  slice->elem_size = elem_size;
}

void slice_free( slice_t *slice ) {
  slice->size = 0;
  slice->cap = 0;
  alloc_free( slice->data );
}

size_t slice_bytes( slice_t slice ) {
  return slice.size * slice.elem_size;
}

size_t slice_size( slice_t slice ) {
  return slice.size;
}

void * slice_ptr(slice_t slice) {
  return slice.data;
}

void * slice_idx(slice_t slice, int idx) {
  return (void *)((char *)slice.data + idx * slice.elem_size);
}

void slice_push( slice_t *slice, const void* elem) {
  slice->data = alloc_push( slice->data, &slice->size, &slice->cap, elem, slice->elem_size );
}

#endif