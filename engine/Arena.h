#ifndef ARENA_H
#define ARENA_H

#include "unity.h"
#include <stdio.h>
#include <stdlib.h>

struct ArenaPtr {
  void *ptr;
  int cap;
};

internal ArenaPtr ArenaConstruct(int cap) {
  ArenaPtr arena;
  void *ptr = malloc(cap);
  arena.ptr = ptr;

  if (ptr == NULL) {
    printf("ArenaConstruct failed: malloc failed to allocate %zu bytes\n", cap);
    arena.cap = cap;
  } else {
    arena.cap = 0;
  }

  return arena;
}

#endif