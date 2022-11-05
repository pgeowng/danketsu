#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>

#define internal static
#define local_persist static
#define global_variable static

int g_screenWidth = 640;
int g_screenHeight = 480;
SDL_Window *g_window;
SDL_GLContext g_ctx;

GLuint g_programID = 0;
GLint g_vertexPos2DLocation = -1;
GLuint g_vbo = 0;
GLuint g_ibo = 0;

internal void printProgramLog(GLuint program)
{
  if (!glIsProgram(program))
  {
    printf("print program log failed: %d isn't program\n", program);
    return;
  }

  int infoLogLength = 0;
  int maxLength = infoLogLength;

  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

  char *infoLog = new char[maxLength];
  glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
  if (infoLogLength > 0)
  {
    printf("%s\n", infoLog);
  }

  delete[] infoLog;
}

internal void printShaderLog(GLuint shader)
{
  if (!glIsShader(shader))
  {
    printf("print shader log failed: %d isn't shader\n", shader);
    return;
  }

  int infoLogLength = 0;
  int maxLength = infoLogLength;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

  char *infoLog = new char[maxLength];
  glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
  if (infoLogLength > 0)
  {
    printf("%s\n", infoLog);
  }

  delete[] infoLog;
}

internal void render()
{
  glViewport(0, 0, g_screenWidth, g_screenHeight);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(g_window);

  // glUseProgram(g_programID);
  // glEnableVertexAttribArray(g_vertexPos2DLocation);

  // glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  // glVertexAttribPointer(g_vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
  // glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

  // glDisableVertexAttribArray(g_vertexPos2DLocation);

  // glUseProgram(NULL);
}

internal bool initGL()
{

  // g_programID = glCreateProgram();
  // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // const GLchar *vertex_shader_source[] = {
  //     "#version 140\nin vec2 LVertexPos2D; void main() {gl_Position = vec4();}"};

  // glShaderSource(vertexShader, 1, vertex_shader_source, NULL);
  // glCompileShader(vertexShader);

  // GLint vertexShaderCompiled = GL_FALSE;
  // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
  // if (vertexShaderCompiled != GL_TRUE)
  // {
  //   printf("compile vertex shader failed: %d\n", vertexShader);
  //   printShaderLog(vertexShader);
  //   return false;
  // }

  // glAttachShader(g_programID, vertexShader);
  // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // const GLchar *fragment_shader_source[] = {
  //     "#version 140\nout vec4 LFragment; void main() {LFragment = vec4(1.0, 1.0, 1.0, 1.0);}"};

  // glShaderSource(fragmentShader, 1, fragment_shader_source, NULL);
  // glCompileShader(fragmentShader);

  // GLint fragment_shader_compiled = GL_FALSE;
  // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_shader_compiled);
  // if (fragment_shader_compiled != GL_TRUE)
  // {
  //   printf("compile fragment shader failed: %d\n", fragmentShader);
  //   printShaderLog(fragmentShader);
  //   return false;
  // }

  // glAttachShader(g_programID, fragmentShader);
  // glLinkProgram(g_programID);

  // GLint programSuccess = GL_TRUE;
  // glGetProgramiv(g_programID, GL_LINK_STATUS, &programSuccess);
  // if (programSuccess != GL_TRUE)
  // {
  //   printf("link shader program failed: %d\n", g_programID);
  //   printProgramLog(g_programID);
  //   return false;
  // }

  // g_vertexPos2DLocation = glGetAttribLocation(g_programID, "LVertexPos2D");
  // if (g_vertexPos2DLocation == -1)
  // {
  //   printf("get LVertexPos2D failed\n");
  //   return false;
  // }

  // glClearColor(0.f, 0.f, 0.f, 1.f);

  // GLfloat vertexData[] = {
  //     -.5f,
  //     -.5f,

  //     .5f,
  //     -.5f,

  //     .5f,
  //     .5f,

  //     -.5f,
  //     .5f,
  // };

  // GLuint indexData[] = {0, 1, 2, 3};

  // glGenBuffers(1, &g_vbo);
  // glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  // glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

  // glGenBuffers(1, &g_ibo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

  return true;
}

void triangleExampleVBO()
{
  float vertices[] = {
    -.5f,
    .5f,
    0.f,

    .5f,
    -.5f,
    0.f,

    0.0f,
    .5f,
    0.f
  };

  glGenBuffers(1, &g_vbo);
  glBind
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

  if (!initGL())
  {
    printf("init gl failed\n");
    return false;
  }

  return true;
}

internal void clean()
{
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}

int main()
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
}
