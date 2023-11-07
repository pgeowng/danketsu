#ifndef BSABER_APP_H
#define BSABER_APP_H

#include "emos.h"

#include "jsmn.h"

#include "unity.h"

#include "base_types.h"

#include "renderer.h"
#include "renderer_helper.h"

#include "flycamera.h"

#include "memory/base_memory.h"
#include "memory/base_memory_malloc.cpp"

#include "info_dat.cpp"
#include "storage.cpp"

typedef struct app {
  RendererEx r;

} app;

static bool appInit(app *p);
static void appUpdate(app *p, f32 delta);
static void appInput(app *p, SDL_Event e);
static void appClean(app *p);

static void appRealUpdate(app *p, f32 delta);
static void appDraw3D(app *p);

#endif
