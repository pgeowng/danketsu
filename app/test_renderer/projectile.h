#ifndef PROJECTILE_H
#define PROJECTILE_H
struct Projectile;

typedef struct Projectile {
  b8 enable;

  f32 position[4];
  u32 speed;
  u32 entityTarget;

  f32 lastTargetKnownPosition[2];

  // _projectilePoolNextFree is next free projectile object. Field is required
  // by ProjectilePool.
  Projectile *_projectilePoolNextFree;
  b8 _projectilePoolHasNextFree;
} Projectile;

typedef struct ProjectilePool {
  u32 projectileIdx;
  u32 cap;
  Projectile proj[48];
  u32 lastCreatedProjectileIndex;

  Projectile *head;
  b8 firstFree;

} ProjectilePool;

void ProjectilePoolInit(ProjectilePool *pool) {
  pool->cap = 48;
  pool->firstFree = 0;
  pool->head = pool->proj;

  for (u32 i = 0; i < pool->cap; i++) {
    if (i + 1 == pool->cap) {
      pool->proj[i]._projectilePoolHasNextFree = 0;
      pool->proj[i]._projectilePoolNextFree = NULL;
      continue;
    }

    pool->proj[i]._projectilePoolHasNextFree = 1;
    pool->proj[i]._projectilePoolNextFree = &pool->proj[i + 1];
  }
}

Projectile *ProjectilePoolAlloc(ProjectilePool *pool) {
  Projectile *ptr = pool->head;

  // NOTE: Always has space for next allocation.
  assert(pool->head->_projectilePoolHasNextFree);

  pool->head = pool->head->_projectilePoolNextFree;
  ptr->_projectilePoolHasNextFree = 0;
  ptr->_projectilePoolNextFree = NULL;

  return ptr;
}

void ProjectilePoolFree(ProjectilePool *pool, Projectile *p) {
  // NOTE: assert that pointer is inside pools range.
  assert(p > pool->proj && p < pool->proj + pool->cap);
  // NOTE: doesn't have next free. aka wasn't deallocated before.
  assert(!p->_projectilePoolHasNextFree);

  p->_projectilePoolNextFree = pool->head;
  p->_projectilePoolHasNextFree = 1;
  pool->head = p;
}
#endif