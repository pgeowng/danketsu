#ifndef PROJECTILE_H
#define PROJECTILE_H
struct Projectile;

typedef struct Projectile {

  f32 position[4];
  u32 speed;
  u32 entityTarget;

  // rotation is a rotation of sprite that is being dispayed.
  f32 rotation;

  f32 lastTargetKnownPosition[2];

  // _projectilePoolNextFree is next free projectile object. Field is required
  // by ProjectilePool.
  Projectile *_projectilePoolNextFree;

  // _projectilePoolHasNextFree is indicator that is next ptr is presented.
  b8 _projectilePoolHasNextFree;
  b8 alive;
} Projectile;

typedef struct ProjectilePool {
  // occ is how many slots are occupied by allocations already. i.e. max(pool)
  u32 occ;

  // cap is how many slots are available until memory overflow.
  u32 cap;

  // pool is allocated on stack chunk of memory.
  Projectile pool[128];

  // head is ptr at first free element.
  Projectile *head;

} ProjectilePool;

void ProjectilePoolInit(ProjectilePool *pool) {
  pool->occ = 0;
  pool->cap = 128;
  pool->head = pool->pool;

  for (u32 i = 0; i < pool->cap; i++) {
    if (i + 1 == pool->cap) {
      pool->pool[i]._projectilePoolHasNextFree = 0;
      pool->pool[i]._projectilePoolNextFree = NULL;
      continue;
    }

    pool->pool[i]._projectilePoolHasNextFree = 1;
    pool->pool[i]._projectilePoolNextFree = &pool->pool[i + 1];
  }
}

Projectile *ProjectilePoolAlloc(ProjectilePool *pool) {
  Projectile *p = pool->head;

  u32 newOcc = (u32)(p - pool->pool) + 1;
  if (pool->occ < newOcc) {
    pool->occ = newOcc;
  }

  // NOTE: Always has space for next allocation.
  assert(pool->head != NULL);

  pool->head = pool->head->_projectilePoolNextFree;
  p->_projectilePoolHasNextFree = 0;
  p->_projectilePoolNextFree = NULL;
  p->alive = 1;

  return p;
}

void ProjectilePoolFree(ProjectilePool *pool, Projectile *p) {
  // NOTE: assert that pointer is inside pools range.
  assert(p >= pool->pool && p < pool->pool + pool->cap);
  // NOTE: doesn't have next free. aka wasn't deallocated before.
  assert(!p->_projectilePoolHasNextFree);

  p->_projectilePoolNextFree = pool->head;
  p->_projectilePoolHasNextFree = 1;
  p->alive = 0;

  pool->head = p;
}

b8 ProjectilePoolCanAlloc(ProjectilePool *pool) { return pool->head != NULL; }

#endif