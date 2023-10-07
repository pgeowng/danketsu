#include "test_renderer.h"

static bool app_init(Scene *app) {
  RenderExInit(&app->r);
  RenderTextureLoad(&app->textures[0], "assets/sprite1.png");
  return true;
}
static void app_update(Scene *app, float delta) {
  RendererEx *rx = &app->r;
  Renderer *r = &rx->r;
  RenderBeginFrame(r);

  u32 spriteWidth = 113;
  u32 spriteHeight = 128;

  float a[2] = {0, 0};
  float b[2] = {(f32)(spriteWidth), 0};
  float c[2] = {0, (f32)(spriteHeight)};
  float ac[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  float bc[4] = {0.0f, 1.0f, 0.0f, 1.0f};
  float cc[4] = {0.0f, 0.0f, 1.0f, 1.0f};
  float aUV[2] = {0.0f, 0.0f};
  float bUV[2] = {(f32)(spriteWidth) / (app->textures[0].width), 0.0f};
  float cUV[2] = {0.0f, (f32)(spriteHeight) / (app->textures[0].height)};
  u32 texture = app->textures[0].texture;

  RenderPushTriangle(r, a, b, c, ac, bc, cc, aUV, bUV, cUV, texture);

  f32 pos[4] = {750.0f, 750.0f, 100.0f, 100.0f};
  // f32 col[4] = {1.0f, 0.5f, 0.5f, 1.0f};

  // RenderPushQuadColor(rx, pos, col);

  f32 white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  RenderPushQuadTex(rx, pos, app->textures[0].texture, white);

  f32 posx[4] = {100.0f, 100.0f, (f32)spriteWidth, (f32)spriteHeight};
  f32 uvx[4] = {0.0f, 0.0f, (f32)(spriteWidth) / (app->textures[0].width),
                (f32)(spriteHeight) / (app->textures[0].height)};
  RenderPushQuadSubTex(rx, posx, uvx, app->textures[0].texture, white);

  RenderEndFrame(r);
}
static void app_input(Scene *app, SDL_Event e) {}
static void AppClean(Scene *app) { RenderFree(&app->r.r); }