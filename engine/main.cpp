#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>

#include "shader.h"

#define internal static
#define local_persist static
#define global_variable static

int g_screenWidth = 640;
int g_screenHeight = 480;
SDL_Window *g_window;
SDL_GLContext g_ctx;

GLint g_vertexPos2DLocation = -1;
GLuint g_vao = 0;
GLuint g_vbo = 0;
GLuint g_ebo = 0;

GLuint g_vao1 = 0;
GLuint g_vbo1 = 0;

shader_s g_shaders[2] = {{0}, {0}};

void initTwoVAO()
{
  float triangle1[] = {
      // first bottom left
      -.5f,
      0.0f,
      0.0f,

      1.0f,
      0.0f,
      0.0f,

      // first top left
      -.5f,
      0.5f,
      0.0f,

      0.0f,
      1.0f,
      0.0f,

      // first bottom right
      -.25f,
      0.0f,
      0.0f,

      0.0f,
      0.0f,
      1.0f};

  float triangle2[] = {
      // second top left
      0.0f,
      0.5f,
      0.0f,

      1.0f,
      0.0f,
      0.0f,

      // second top right
      .25f,
      0.5f,
      0.0f,

      0.0f,
      1.0f,
      0.0f,

      // second bottom right
      .25f,
      0.0f,
      0.0f,

      0.0f,
      0.0f,
      1.0f};

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);
  glGenBuffers(1, &g_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenVertexArrays(1, &g_vao1);
  glBindVertexArray(g_vao1);
  glGenBuffers(1, &g_vbo1);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void renderTwoVAO()
{

  float timeValue = SDL_GetTicks() / 1000.0f;
  float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

  shader_Use(&g_shaders[0]);
  shader_Set4f(&g_shaders[0], "ourColor", 0.0f, greenValue, 0.0f, 1.0f);

  glBindVertexArray(g_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  shader_Use(&g_shaders[1]);
  glBindVertexArray(g_vao1);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

internal void render()
{
  glViewport(0, 0, g_screenWidth, g_screenHeight);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  renderTwoVAO();
  // glBindVertexArray(g_vao);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // glDrawArrays(GL_TRIANGLES, 0, 8);

  SDL_GL_SwapWindow(g_window);

  // glUseProgram(g_shaderProgram);
  // glEnableVertexAttribArray(g_vertexPos2DLocation);

  // glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  // glVertexAttribPointer(g_vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
  // glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

  // glDisableVertexAttribArray(g_vertexPos2DLocation);

  // glUseProgram(NULL);
}

internal bool initTwoShaders()
{

  bool ok = false;
  ok = shader_New(&g_shaders[0], "./engine/shaders/hello.vert", "./engine/shaders/hello.frag");
  if (!ok)
  {
    printf("failed to create shader program 0");
    return ok;
  }

  ok = shader_New(&g_shaders[1], "./engine/shaders/hello.vert", "./engine/shaders/colorful.frag");
  if (!ok)
  {
    printf("failed to create shader program 1");
    return ok;
  }

  glPolygonMode(GL_FRONT_LEFT, GL_LINE);

  return true;
}

void triangleExampleVBO()
{
  // float vertices[] = {
  //     // bottom left
  //     -.5f,
  //     -.5f,
  //     0.f,

  //     // bottom right
  //     .5f,
  //     -.5f,
  //     0.f,

  //     // top left
  //     -0.5f,
  //     0.5f,
  //     0.0f,

  //     // top right
  //     0.5f,
  //     0.5f,
  //     0.0f};

  float vertices[] = {
      // first bottom left
      -.5f,
      0.0f,
      0.0f,

      // first top left
      -.5f,
      0.5f,
      0.0f,

      // first bottom right
      -.25f,
      0.0f,
      0.0f,

      // second top left
      0.0f,
      0.5f,
      0.0f,

      // second top right
      .25f,
      0.5f,
      0.0f,

      // second bottom right
      .25f,
      0.0f,
      0.0f};

  unsigned int indices[] = {
      0, 1, 2,
      3, 4, 5};

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);

  glGenBuffers(1, &g_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &g_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

internal bool
init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("sdl init failed: %s\n", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  g_window = SDL_CreateWindow(
      "Danketsu",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      g_screenWidth,
      g_screenHeight,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if (g_window == NULL)
  {
    printf("sdl create window failed: %s\n", SDL_GetError());
    return false;
  }

  g_ctx = SDL_GL_CreateContext(g_window);
  if (g_ctx == NULL)
  {
    printf("sdl gl create context failed: %s\n", SDL_GetError());
    return false;
  }

  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK)
  {
    printf("glew init failed: %s\n", glewGetErrorString(glewError));
    return false;
  }

  if (SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("sdl gl set swap interval: %s\n", SDL_GetError());
    return false;
  }

  if (!initTwoShaders())
  {
    printf("init gl failed\n");
    return false;
  }

  initTwoVAO();

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  printf("Maximum nr of vertex attributes supported: %d", nrAttributes);

  return true;
}

internal void clean()
{
  glDeleteVertexArrays(1, &g_vao1);
  glDeleteVertexArrays(1, &g_vao);
  glDeleteBuffers(1, &g_vbo);
  glDeleteBuffers(1, &g_vbo1);

  shader_Delete(&g_shaders[0]);
  shader_Delete(&g_shaders[1]);

  SDL_GL_DeleteContext(g_ctx);
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}

int main(int argc, char *argv[])
{

  bool ok = init();
  if (!ok)
  {
    printf("init failed\n");
    clean();
    return 0;
  }

  SDL_Event e;
  bool windowShouldClose = false;
  while (!windowShouldClose)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        windowShouldClose = true;
      }
    }

    render();
  }

  clean();

  return 0;
}
