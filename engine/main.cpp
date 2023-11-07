#include "unity.h"

#include "flycamera.h"
#include "shader.h"

// #include "../app/hello/hello.cpp"
// #include "../app/hello/input.h"
// #include "../app/audio/audio.cpp"
// #include "../app/light/init.cpp"
// #include "../app/light/input.cpp"
// #include "../app/light/render.cpp"

// #include "../app/match3/match3.cpp"
#include "../app/app.h"

int g_screenWidth = 1000;
int g_screenHeight = 1000;
SDL_Window *g_window;
SDL_GLContext g_ctx;

float prevTime = 0;

internal bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("sdl init failed: %s\n", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  g_window = SDL_CreateWindow("Danketsu", 0, 20, g_screenWidth, g_screenHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if (g_window == NULL) {
    printf("sdl create window failed: %s\n", SDL_GetError());
    return false;
  }

  g_ctx = SDL_GL_CreateContext(g_window);
  if (g_ctx == NULL) {
    printf("sdl gl create context failed: %s\n", SDL_GetError());
    return false;
  }

  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    printf("glew init failed: %s\n", glewGetErrorString(glewError));
    return false;
  }

  if (SDL_GL_SetSwapInterval(0) < 0) {
    printf("sdl gl set swap interval: %s\n", SDL_GetError());
    return false;
  }

  // glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!mainInit()) {
    printf("mainInit failed");
    return false;
  }

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  printf("Maximum nr of vertex attributes supported: %d\n", nrAttributes);

  return true;
}

internal void clean() {
  mainClean();
  SDL_GL_DeleteContext(g_ctx);
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}

#define TICK_INTERVAL 2
static Uint32 nextTime;

Uint32 timeLeft() {
  Uint32 now;
  now = SDL_GetTicks();
  if (nextTime <= now) {
    return 0;
  } else {
    return nextTime - now;
  }
}

int run() {
  bool ok = init();
  if (!ok) {
    printf("init failed\n");
    clean();
    return 0;
  }

  int input_move_forward = 0;
  int input_move_right = 0;
  int input_move_left = 0;
  int input_move_back = 0;

  SDL_Event e;
  bool windowShouldClose = false;

  // Performance Metrics
  u32 lastTime = SDL_GetTicks();
  int nbFrames = 0;

  while (!windowShouldClose) {

    {
      u32 currentTime = SDL_GetTicks();
      nbFrames++;
      if (currentTime - lastTime >= 1000) {
        LogInfo("%f ms/frame", 1000.0 / float(nbFrames));
        nbFrames = 0;
        lastTime += 1000;
      }
    }

    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT:
        windowShouldClose = true;
        break;
      case SDL_WINDOWEVENT: {
        if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
          g_screenWidth = e.window.data1;
          g_screenHeight = e.window.data2;
        }
        break;
      }
      }

      mainInput(e);
    }

    float currentTime = SDL_GetTicks() / 1000.0f;
    float delta = currentTime - prevTime;
    prevTime = currentTime;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainUpdate(delta);

    SDL_GL_SwapWindow(g_window);

    nextTime = SDL_GetTicks() + TICK_INTERVAL;
    SDL_Delay(timeLeft());
    nextTime += TICK_INTERVAL;
  }

  clean();

  return 0;
}
