#define TOWER_TILE_SIZE 64.0f
#define TOWER_ARROW_ID 0

typedef struct Tower {
  b8 enable;

  f32 position[4];
  i32 tileIdx;

  u32 lastShootTick;
  u32 shootCooldown;
  u32 detectRangeSquared;
  u32 detectRange;

  // Positions from which all projectiles are created. Offset relative to
  // top-left corner.
  u32 spawnPosition[2];
  // That distance from spawnPosition is used for creating projectiles toward
  // enemies.
  u32 spawnRange;

} Tower;

void towerNew(Tower *t, v2 spawnPosition) {
  t->enable = 1;

  v2Copy(t->position, spawnPosition);
  t->position[2] = TOWER_TILE_SIZE;
  t->position[3] = TOWER_TILE_SIZE;

  t->tileIdx = TOWER_ARROW_ID;
  t->lastShootTick = 0;
  t->shootCooldown = 750;
  t->detectRange = 200;
  t->detectRangeSquared = t->detectRange * t->detectRange;

  t->spawnPosition[0] = 32;
  t->spawnPosition[1] = 32;

  t->spawnRange = 32;
}