#ifndef BSABER_APP_H
#define BSABER_APP_H

#include "emos.h"

#include "jsmn.h"

#include "unity.h"

#include "base_types.h"

#include "memory/base_memory.h"
#include "memory/base_memory_malloc.cpp"

typedef struct app {

} app;

static bool appInit(app *p);
static void appUpdate(app *p, f32 delta);
static void appInput(app *p, SDL_Event e);
static void appClean(app *p);

#endif
