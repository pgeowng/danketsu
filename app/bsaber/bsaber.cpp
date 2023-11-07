#include "bsaber.h"

static Camera camera = {};

static bool appInit(app *p) {
  emArenaInit();
  MArena *a = emArena;

  b8 err = loadStorage(a);
  if (err) {
    LogError("loadStorage failed");
  }

  RenderExInit(&p->r);
  cameraInit(&camera, 45.0f, 1.0f, 0.1f, 100.0f, 1);

  return 1;
}
static void appUpdate(app *p, f32 delta) {
  appRealUpdate(p, delta);
  appDraw3D(p);
}

static void appInput(app *p, SDL_Event e) {
  switch (e.type) {
  case SDL_MOUSEMOTION: {
    flycamera_process_mouse_movement(&camera, (float)(e.motion.xrel),
                                     (float)(e.motion.yrel));
    break;
  }
  }
}
static void appClean(app *p) {}
static void appRealUpdate(app *p, f32 delta) {
  flycamera_update(&camera, delta);
}

static void appDraw3D(app *p) {
  RendererEx *rx = &p->r;

  RenderBeginFrame(&rx->r);

  f32 a[3] = {-1.0f, 0.0f, -1.0f};
  f32 b[3] = {1.0f, 0.0f, -1.0f};
  f32 c[3] = {-1.0f, 0.0f, 1.0f};
  f32 d[3] = {1.0f, 0.0f, 1.0f};
  RenderPush3DQuadColor(rx, a, b, c, d, colorRed);

  camProjM4(&camera, rx->r.projection);

  RenderEndFrame(&rx->r);
}