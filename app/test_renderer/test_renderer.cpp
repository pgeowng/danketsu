#include "test_renderer.h"

#define CURSOR_TEXTURE_ID 16
#define TOWER_ARROW_IDX 0

static bool app_init(Scene *app) {
  RenderExInit(&app->r);
  RenderTextureLoad(&app->textures[0], "assets/sprite1.png");
  // jsmn_parser parser;
  // jsmntok_t tokens[10];

  // jsmn_init(&parser);
  // jsmn_parse(&parser, jsonString, jsonStringLen, tokens, 10)

  u32 tileWidth = 64;
  u32 tileHeight = 64;
  RenderTextureLoad(&app->textures[1], "assets/tower1.png");

  TileMapLoadCSV(&app->background, "assets/map1_background.csv",
                 &app->textures[1], tileWidth, tileHeight);
  TileMapLoadCSV(&app->shadows, "assets/map1_shadows.csv", &app->textures[1],
                 tileWidth, tileHeight);

  return true;
}

static void app_update(Scene *app, float delta) {

  f32 tileWidth = 64.0f;
  f32 tileHeight = 64.0f;

  // update
  i32 mousePosition[2];
  u32 mouseState = SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);

  b8 mouseLeftPrev = app->mouseLeftPrev;
  b8 mouseLeftActive = !!(SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState);

  f32 cursorPosition[4] = {0.0f, 0.0f, tileWidth, tileHeight};
  cursorPosition[0] = (i32)(mousePosition[0] / tileWidth) * tileWidth;
  cursorPosition[1] = (i32)(mousePosition[1] / tileHeight) * tileHeight;

  f32 cursorUV[4] = {0.0f};
  TileMapUVByIdx(cursorUV, &app->background, CURSOR_TEXTURE_ID);

  if (mouseLeftActive && !mouseLeftPrev) {
    LogInfo("create tower event");
    Tower *instance = &app->towers[app->towersIdx];
    instance->enable = 1;
    v4Copy(instance->position, cursorPosition);
    instance->tileIdx = TOWER_ARROW_IDX;
    app->towersIdx++;
  }

  // update defer

  app->mouseLeftPrev = mouseLeftActive;

  // draw
  Texture *cursorTexture = &app->textures[1];

  RendererEx *rx = &app->r;
  Renderer *r = &rx->r;
  RenderBeginFrame(r);

  u32 spriteWidth = 113;
  u32 spriteHeight = 128;

  // float a[2] = {0, 0};
  // float b[2] = {(f32)(spriteWidth), 0};
  // float c[2] = {0, (f32)(spriteHeight)};
  // float ac[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  // float bc[4] = {0.0f, 1.0f, 0.0f, 1.0f};
  // float cc[4] = {0.0f, 0.0f, 1.0f, 1.0f};
  // float aUV[2] = {0.0f, 0.0f};
  // float bUV[2] = {(f32)(spriteWidth) / (app->textures[0].width), 0.0f};
  // float cUV[2] = {0.0f, (f32)(spriteHeight) / (app->textures[0].height)};
  // u32 texture = app->textures[0].texture;

  // RenderPushTriangle(r, a, b, c, ac, bc, cc, aUV, bUV, cUV, texture);

  // f32 pos[4] = {750.0f, 750.0f, 100.0f, 100.0f};
  // // f32 col[4] = {1.0f, 0.5f, 0.5f, 1.0f};

  // // RenderPushQuadColor(rx, pos, col);

  // RenderPushQuadTex(rx, pos, app->textures[0].texture, white);

  // f32 posx[4] = {100.0f, 100.0f, (f32)spriteWidth, (f32)spriteHeight};
  // f32 uvx[4] = {0.0f, 0.0f, (f32)(spriteWidth) / (app->textures[0].width),
  //               (f32)(spriteHeight) / (app->textures[0].height)};
  // RenderPushQuadSubTex(rx, posx, uvx, app->textures[0].texture, white);

  DrawTileMapLayer(rx, &app->background);
  DrawTileMapLayer(rx, &app->shadows);

  Texture *towerTexture = &app->textures[1];
  f32 towerPosition[4] = {128.0f, 128.0f, 64.0f, 64.0f};
  f32 towerUVs[4] = {0.0f, 0.0f, 64.0f / (towerTexture->width),
                     64.0f / (towerTexture->height)};
  RenderPushQuadSubTex(rx, towerPosition, towerUVs, towerTexture->texture,
                       colorWhite);

  // draw cursor
  RenderPushQuadSubTex(rx, cursorPosition, cursorUV, cursorTexture->texture,
                       colorWhite);

  for (u32 i = 0; i < ArrayCount(app->towers); i++) {
    Tower *t = &app->towers[i];
    if (!t->enable)
      continue;

    f32 uv[4] = {0.0f};
    TileMapUVByIdx(uv, &app->background, t->tileIdx);

    RenderPushQuadSubTex(rx, t->position, uv, towerTexture->texture,
                         colorWhite);
  }

  RenderEndFrame(r);
}
static void app_input(Scene *app, SDL_Event e) {
  // switch (e.type) {
  // case SDL_MOUSEMOTION:
  // i32 x, y;
  // SDL_GetMouseState(&x, &y);
  // LogInfo("event=(%d, %d) state=(%d, %d)", e.motion.x, e.motion.y, x, y);
  // }
}
static void AppClean(Scene *app) { RenderFree(&app->r.r); }

static void TileMapUVByIdx(v4 uvOut, TileMap *tm, i32 idx) {
  f32 tileWidth = (f32)(tm->tileWidth);
  f32 tileHeight = (f32)(tm->tileHeight);
  u32 maxX = (u32)(tm->texture->width / tileWidth);

  uvOut[0] = idx % maxX * tileWidth / tm->texture->width;
  uvOut[1] = idx / maxX * tileHeight / tm->texture->height;
  uvOut[2] = tileWidth / tm->texture->width;
  uvOut[3] = tileHeight / tm->texture->height;
}

static void DrawTileMapLayer(RendererEx *rx, TileMap *tm) {
  f32 tileWidth = (f32)(tm->tileWidth);
  f32 tileHeight = (f32)(tm->tileHeight);

  for (u32 row = 0; row < tm->height; row++) {
    for (u32 col = 0; col < tm->width; col++) {
      i32 tile = tm->map[row][col];
      if (tile == -1) {
        continue;
      }

      f32 tilePosition[4] = {(f32)(col * tileWidth), (f32)(row * tileHeight),
                             tileWidth, tileHeight};

      f32 tileUV[4] = {0.0f};
      TileMapUVByIdx(tileUV, tm, tile);
      RenderPushQuadSubTex(rx, tilePosition, tileUV, tm->texture->texture,
                           colorWhite);
    }
  }
}