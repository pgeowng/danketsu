// #include "../app/test_renderer/test_renderer.cpp"

// Hello App
#include "../app/hello/hello.cpp"

Scene g_app = {};

static bool mainInit() { return app_init(&g_app); }
static void mainClean() {  }
static void mainInput(SDL_Event e) {  }
static void mainUpdate(f32 delta) { app_update(&g_app, delta); }



// #include "../app/hello/input.h"
// #include "../app/audio/audio.cpp"
// #include "../app/light/init.cpp"
// #include "../app/light/input.cpp"
// #include "../app/light/render.cpp"

// #include "../app/match3/match3.cpp"

// tower defence game
// #include "../app/test_renderer/test_renderer.cpp"
// Scene g_app = {};

// static bool mainInit() { return app_init(&g_app); }
// static void mainClean() { AppClean(&g_app); }
// static void mainInput(SDL_Event e) { app_input(&g_app, e); }
// static void mainUpdate(f32 delta) { app_update(&g_app, delta); }

// beat saber app
// #include "memory/emarena.cpp"

// #include "bsaber/bsaber.cpp"

// app g_app = {};

// static bool mainInit() { return appInit(&g_app); }
// static void mainClean() { appClean(&g_app); }
// static void mainInput(SDL_Event e) { appInput(&g_app, e); }
// static void mainUpdate(f32 delta) { appUpdate(&g_app, delta); }
