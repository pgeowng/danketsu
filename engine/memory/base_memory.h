#include "unity.h"

#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

typedef void *MReserveFunc(void *ctx, U64 size);
typedef void MChangeMemoryFunc(void *ctx, void *ptr, U64 size);

struct MBaseMemory {
  MReserveFunc *reserve;
  MChangeMemoryFunc *commit;
  MChangeMemoryFunc *decommit;
  MChangeMemoryFunc *release;
  void *ctx;
};

internal void MChangeMemoryNoop(void *ctx, void *ptr, U64 size);
// TODO: define in cpp
internal void MChangeMemoryNoop(void *ctx, void *ptr, U64 size) {}

struct MArena {
  MBaseMemory *base;
  U8 *memory;
  U64 cap;
  U64 pos;
  U64 commitPos;
};

struct MTemp {
  MArena *arena;
  U64 pos;
};

struct MTempBlock {
  MTemp temp;

  MTempBlock(MArena *arena);
  ~MTempBlock();
  void reset();
};

#define M_DEFAULT_RESERVE_SIZE GB(1)
#define M_COMMIT_BLOCK_SIZE MB(64)

internal MArena MArenaMakeReserve(MBaseMemory *base, U64 reserveSize);
internal MArena MArenaMake(MBaseMemory *base);

internal void MArenaRelease(MArena *arena);
internal void *MArenaPush(MArena *arena, U64 size);
internal void MArenaPopTo(MArena *arena, U64 pos);

internal void *MArenaPushZero(MArena *arena, U64 size);
internal void MArenaAlign(MArena *arena, U64 pow2Align);
internal void MArenaAlign(MArena *arena, U64 pow2Align);

#define PushArray(a, T, c) (T *)MArenaPush((a), sizeof(T) * (c))
#define PushArrayZero(a, T, c) (T *)MArenaPushZero((a), sizeof(T) * (c))

internal MTemp MTempBegin(MArena *arena);
internal void MTempEnd(MTemp temp);

#endif