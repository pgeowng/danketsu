#include "test_renderer.h"

static bool app_init(Scene *app) {
  RenderInit(&app->r);
  return true;
}
static void app_update(Scene *app, float delta) {
  Renderer *r = &app->r;
  RenderBeginFrame(r);

  float a[2] = {0, 0};
  float b[2] = {0, 10};
  float c[2] = {10, 0};
  float ac[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  float bc[4] = {0.0f, 1.0f, 0.0f, 1.0f};
  float cc[4] = {0.0f, 0.0f, 1.0f, 1.0f};

  RenderPushTriangle(r, a, b, c, ac, bc, cc);
  RenderEndFrame(r);
}
static void app_input(Scene *app, SDL_Event e) {}
static void AppClean(Scene *app) { RenderFree(&app->r); }