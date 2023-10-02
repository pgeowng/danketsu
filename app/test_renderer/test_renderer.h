#ifndef TEST_RENDERER_H
#define TEST_RENDERER_H

#include "renderer.h"

typedef struct Scene {
  Renderer r;
} Scene;

static bool app_init(Scene *app);
static void app_update(Scene *app, float delta);
static void app_input(Scene *app, SDL_Event e);
static void AppClean(Scene *app);

#endif