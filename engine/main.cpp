#include "unity.h"

#include "flycamera.h"
#include "shader.h"

// #include "../app/hello/hello.cpp"
#include "../app/audio/audio.cpp"
// #include "../app/light/light.cpp"

int g_screenWidth = 1000;
int g_screenHeight = 600;
SDL_Window* g_window;
SDL_GLContext g_ctx;

float prevTime = 0;

app_s g_app = {};

internal bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("sdl init failed: %s\n", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  g_window = SDL_CreateWindow(
      "Danketsu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_screenWidth,
      g_screenHeight,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

  glEnable(GL_DEPTH_TEST);

  if (!app_init(&g_app)) {
    printf("cubes init failed\n");
    return false;
  }

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  printf("Maximum nr of vertex attributes supported: %d\n", nrAttributes);

  return true;
}

internal void clean() {
  SDL_GL_DeleteContext(g_ctx);
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}

int main(int argc, char* argv[]) {

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
  while (!windowShouldClose) {
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
      app_input(&g_app, e);
    }

    float timeValue = SDL_GetTicks() / 1000.0f;
    float delta = timeValue - prevTime;
    prevTime = timeValue;

    glClearColor(0.15625f, 0.15625f, 0.15625f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app_update(&g_app, delta);

    SDL_GL_SwapWindow(g_window);
  }

  clean();

  return 0;
}
