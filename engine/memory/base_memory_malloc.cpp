#include "base_memory.h"
#include "unity.h"

#ifndef BASE_MEMORY_MALLOC_CPP
#define BASE_MEMORY_MALLOC_CPP

internal void *MMallocReserve(void *ctx, U64 size) { return malloc(size); }

internal void MMallocRelease(void *ctx, void *ptr, U64 size) { free(ptr); }

internal MBaseMemory *MMallocBaseMemory() {
  local_persist MBaseMemory memory = {};

  if (memory.reserve == 0) {
    memory.reserve = MMallocReserve;
    memory.commit = MChangeMemoryNoop;
    memory.decommit = MChangeMemoryNoop;
    memory.release = MMallocRelease;
  }

  return &memory;
}

internal MArena MArenaMakeReserve(MBaseMemory *base, U64 reserveSize) {
  MArena result = {};

  result.base = base;
  result.memory = (U8 *)base->reserve(base->ctx, reserveSize);
  result.cap = reserveSize;

  return result;
}

internal MArena MArenaMake(MBaseMemory *base) {
  MArena result = MArenaMakeReserve(base, M_DEFAULT_RESERVE_SIZE);
  return result;
}

internal void MArenaRelease(MArena *arena) {
  MBaseMemory *base = arena->base;
  base->release(base->ctx, arena->memory, arena->cap);
}

internal void *MArenaPush(MArena *arena, U64 size) {
  void *result = 0;

  if (arena->pos + size <= arena->cap) {
    result = arena->memory + arena->pos;
    arena->pos += size;

    U64 pos = arena->pos;
    U64 commitPos = arena->commitPos;
    if (pos > commitPos) {
      U64 posAligned = AlignUpPow2(pos, M_COMMIT_BLOCK_SIZE - 1);
      U64 nextCommitPos = ClampTop(posAligned, arena->cap);
      U64 commitSize = nextCommitPos - commitPos;

      MBaseMemory *base = arena->base;
      base->commit(base->ctx, arena->memory + commitPos, commitSize);
      arena->commitPos = nextCommitPos;
    }
  }

  return result;
}

internal void MArenaPopTo(MArena *arena, U64 setPos) {
  if (setPos < arena->pos) {
    arena->pos = setPos;

    U64 pos = arena->pos;
    U64 posAligned = AlignUpPow2(pos, M_COMMIT_BLOCK_SIZE - 1);
    U64 nextCommitPos = ClampTop(posAligned, arena->cap);

    U64 commitPos = arena->commitPos;
    if (nextCommitPos < commitPos) {
      U64 decommitSize = commitPos - nextCommitPos;
      MBaseMemory *base = arena->base;
      base->decommit(base->ctx, arena->memory + commitPos, decommitSize);
      arena->commitPos = nextCommitPos;
    }
  }
}

internal void *MArenaPushZero(MArena *arena, U64 size) {
  void *result = MArenaPush(arena, size);
  MemoryZero(result, size);
  return result;
}

internal void MArenaAlign(MArena *arena, U64 pow2Align) {
  U64 pos = arena->pos;
  U64 posAligned = AlignUpPow2(pos, pow2Align);
  U64 z = posAligned - pos;
  if (z > 0) {
    MArenaPush(arena, z);
  }
}

internal void MArenaAlignZero(MArena *arena, U64 pow2Align) {
  U64 pos = arena->pos;
  U64 posAligned = AlignUpPow2(pos, pow2Align);
  U64 z = posAligned - pos;
  if (z > 0) {
    MArenaPushZero(arena, z);
  }
}

// MTemp

internal MTemp MTempBegin(MArena *arena) {
  MTemp temp = {arena, arena->pos};
  return temp;
}

internal void MTempEnd(MTemp temp) { MArenaPopTo(temp.arena, temp.pos); }

// MTempBlock

MTempBlock::MTempBlock(MArena *arena) { this->temp = MTempBegin(arena); }

MTempBlock::~MTempBlock() { MTempEnd(this->temp); }

void MTempBlock::reset() { MTempEnd(this->temp); }

#endif