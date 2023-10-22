#include "projectile.h"

void testFullAlloc() {
  ProjectilePool pool;

  ProjectilePoolInit(&pool);

  assert(pool.head != NULL);
  assert(pool.cap > 0);

  for (u32 i = 0; i + 1 < pool.cap; i++) {
    Projectile *p = ProjectilePoolAlloc(&pool);
    assert(p != NULL);
  }
}

void testAllocFree() {
  ProjectilePool pool1;
  ProjectilePool *pool = &pool1;
  ProjectilePoolInit(pool);

  Projectile *p1 = ProjectilePoolAlloc(pool);
  assert(!p1->_projectilePoolHasNextFree);
  assert(p1->_projectilePoolNextFree == NULL);

  Projectile *p2 = ProjectilePoolAlloc(pool);
  Projectile *p3 = ProjectilePoolAlloc(pool);

  ProjectilePoolFree(pool, p2);
  assert(p2->_projectilePoolHasNextFree);
  assert(p2->_projectilePoolNextFree != NULL);

  ProjectilePoolFree(pool, p3);
  assert(p3->_projectilePoolHasNextFree);
  assert(p3->_projectilePoolNextFree == p2);

  Projectile *p4 = ProjectilePoolAlloc(pool);
  assert(p4 == p3);
  assert(!p4->_projectilePoolHasNextFree);
  assert(p4->_projectilePoolNextFree == NULL);

  Projectile *p5 = ProjectilePoolAlloc(pool);
  assert(p5 == p2);

  Projectile *p6 = ProjectilePoolAlloc(pool);
  assert(p6 != NULL);
}

void runTest() {
  testFullAlloc();
  testAllocFree();
}