#ifndef TEST_RENDERER_H
#define TEST_RENDERER_H

#include "map_loader.cpp"
#include "renderer.h"
#include "renderer_helper.h"

#include "projectile.h"
#include "tower.h"

typedef struct EnemySlime {
  b8 enable;

  f32 position[4];
  i32 bodyTextureIndex;

  i32 eyeTextureIndex;

  u32 pathPointToFollow;

  f32 speed;

} EnemySlime;

typedef struct PathPoint {
  f32 position[2];
} PathPoint;

typedef struct Scene {

  b8 haltNextFrame;

  RendererEx r;
  Texture textures[8];
  TileMap background;
  TileMap shadows;

  TileMap font;

  u32 towersIdx;
  Tower towers[128];

  ProjectilePool _projectilePool;
  ProjectilePool *proj;

  u32 enemySlimeLen;
  u32 enemySlimeCap;
  EnemySlime enemySlime[64];
  u32 lastSpawnTime;

  u32 pathLen;
  PathPoint path[20];

  b8 mouseLeftPrev;

} Scene;

static bool app_init(Scene *app);
static void app_update(Scene *app, float delta);
static void app_input(Scene *app, SDL_Event e);
static void AppClean(Scene *app);

typedef struct Character {
  u32 frameDuration; // milliseconds
} Character;

static void DrawTileMapLayer(RendererEx *rx, TileMap *tm);
static void TileMapUVByIdx(v4 uvOut, TileMap *tm, i32 idx);

#endif