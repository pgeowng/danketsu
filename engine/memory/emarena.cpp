#ifndef EMARENA_H
#define EMARENA_H
#include "base_memory.h"
#include "base_memory_malloc.cpp"

static MArena _emArena;
static MArena *emArena;

static void emArenaInit() {
  _emArena = MArenaMake(MMallocBaseMemory());
  emArena = &_emArena;
}

#endif