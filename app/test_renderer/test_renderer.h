#ifndef TEST_RENDERER_H
#define TEST_RENDERER_H

#include "map_loader.cpp"
#include "renderer.h"
#include "renderer_helper.h"

typedef struct Tower {
  b8 enable;

  f32 position[4];
  i32 tileIdx;

} Tower;

typedef struct Scene {
  RendererEx r;
  Texture textures[8];
  TileMap background;
  TileMap shadows;

  u32 towersIdx;
  Tower towers[16];

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