#include <stdlib.h>
#include <string.h>
#include "debug.h"

typedef uintptr_t UOffset;
typedef unsigned char U8;
typedef unsigned long long U64;



int isPowerOfTwo(UOffset x) {
  return (x & (x-1)) == 0;
}

UOffset alignForward(U64 ptr, U64 align) {
  ASSERT(isPowerOfTwo(align));

  U64 modulo = ptr & (align - 1);

  ASSERT(align > modulo);
  if (modulo != 0) {
    ptr += align - modulo;
  }

  return ptr;
}

typedef struct Arena Arena;
struct Arena {
  U8* buf;
  U64 cap;
  U64 ptr;
  U64 prevPtr;
};

// static
// Arena ArenaMake(U64 size) {
//   Arena a = {};
//   a.buf = (U8*) malloc(size);
//   a.cap = size;
//   a.ptr = 0;
//   a.prevPtr = 0;

//   ASSERT(a.buf != NULL);

//   return a;
// }

static
void * ArenaAllocAlign(Arena *a, U64 size, U64 align) {
  U64 currPtr = (U64)a->buf + align;
  U64 offset = alignForward(currPtr, align);
  offset -= (U64)a->buf;

  if (offset + size <= a->cap) {
    void *ptr = &a->buf[offset];
    a->prevPtr = offset;
    a->ptr = offset+size;

    memset(ptr, 0, size);
    return ptr;
  }

  ASSERT(0 && "Arena: failed to allocate memory")
  return NULL;
}

#ifndef ARENA_DEFAULT_ALIGNMENT
#define ARENA_DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif

static
void* ArenaAlloc(Arena* a, U64 size) {
  return ArenaAllocAlign(a, size, ARENA_DEFAULT_ALIGNMENT);
}


static
void ArenaInit(Arena *a, void* buffer, U64 bufferCapacity) {
  ASSERT(a->buf == NULL);
  ASSERT(buffer != NULL);
  a->buf = (U8*)buffer;
  a->cap = bufferCapacity;
  a->ptr = 0;
  a->prevPtr = 0;
}

static
void ArenaFree(Arena *a, void* ptr) {}

static
void* ArenaResizeAlign(Arena*a, void* oldBuffer, U64 oldSize, U64 newSize, U64 align) {
  U8* oldBuf = (U8*)oldBuffer;

  ASSERT(isPowerOfTwo(align));

  if (oldBuf == NULL || oldSize == 0) {
    return ArenaAllocAlign(a, newSize, align);
  } else if (a->buf > oldBuf && oldBuf >= a->buf + a->cap) {
    ASSERT(0 && "Memory is out of bounds of the buffer in this arena");
    return NULL;
  }

  if (a->buf+a->prevPtr == oldBuf) {
    a->ptr = a->prevPtr + newSize;
    ASSERT((a->ptr < a->cap) && "Requested size is out of bound");
    if (newSize > oldSize) {
      memset(&a->buf[a->prevPtr + oldSize], 0, newSize - oldSize);
    }
    return oldBuffer;
  }

  void* newBuf = ArenaAllocAlign(a, newSize, align);
  U64 copySize = oldSize < newSize ? oldSize : newSize;

  memmove(newBuf, oldBuf, copySize);
  return newBuf;
}

static
void* ArenaResize(Arena *a, void * oldBuffer, U64 oldSize, U64 newSize) {
  return ArenaResizeAlign(a, oldBuffer, oldSize, newSize, ARENA_DEFAULT_ALIGNMENT);
}

static
void ArenaFreeAll(Arena *a) {
  a->ptr = 0;
  a->prevPtr = 0;
}