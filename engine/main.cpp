#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>

#define internal static
#define local_persist static
#define global_variable static

int g_screenWidth = 640;
int g_screenHeight = 480;
SDL_Window *g_window;
SDL_GLContext g_ctx;

GLuint g_shaderProgram = 0;
GLint g_vertexPos2DLocation = -1;
GLuint g_vao = 0;
GLuint g_vbo = 0;
GLuint g_ebo = 0;

GLuint g_vao1 = 0;
GLuint g_vbo1 = 0;

GLuint g_shaderPrograms[2] = {0, 0};

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
  int vertexColorLocation = glGetUniformLocation(g_shaderPrograms[0], "ourColor");
  glUseProgram(g_shaderPrograms[0]);
  glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

  glBindVertexArray(g_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glUseProgram(g_shaderPrograms[1]);
  glBindVertexArray(g_vao1);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

internal void render()
{
  glViewport(0, 0, g_screenWidth, g_screenHeight);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(g_shaderProgram);

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

const char *triangleVertexShaderSource = R"EOL(
  #version 330 core

  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec3 aColor;

  out vec4 vertexColor;

  void main()
  {
      gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
      vertexColor = vec4(aColor, 1.0);
  }
)EOL";

const char *triangleFragmentShaderSource = R"EOL(
  #version 330 core

  in vec4 vertexColor;
  out vec4 FragColor;

  uniform vec4 ourColor;

  void main()
  {
      FragColor = ourColor;
  }
)EOL";

const char *colorfulFragmentShaderSource = R"EOL(
  #version 330 core

in vec4 vertexColor;
  out vec4 FragColor;

  void main()
  {
    FragColor = vertexColor;
  }
)EOL";

internal bool initGL()
{
  GLint ok = GL_FALSE;

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &triangleVertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok);
  if (ok != GL_TRUE)
  {
    printf("compile vertex shader failed: %d\n", vertexShader);
    printShaderLog(vertexShader);
    return false;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &triangleFragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);
  if (ok != GL_TRUE)
  {
    printf("compile fragment shader failed: %d\n", fragmentShader);
    printShaderLog(fragmentShader);
    return false;
  }

  g_shaderProgram = glCreateProgram();
  glAttachShader(g_shaderProgram, vertexShader);
  glAttachShader(g_shaderProgram, fragmentShader);
  glLinkProgram(g_shaderProgram);

  glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &ok);
  if (ok != GL_TRUE)
  {
    printf("link shader program failed: %d\n", g_shaderProgram);
    printProgramLog(g_shaderProgram);
    return false;
  }

  // TODO: in case of error, delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glUseProgram(g_shaderProgram);

  glPolygonMode(GL_FRONT_LEFT, GL_LINE);

  return true;
}

internal bool initTwoShaders()
{
  GLint ok = GL_FALSE;

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  {
    glShaderSource(vertexShader, 1, &triangleVertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE)
    {
      printf("compile vertex shader failed: %d\n", vertexShader);
      printShaderLog(vertexShader);
      return false;
    }
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  {
    glShaderSource(fragmentShader, 1, &triangleFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE)
    {
      printf("compile fragment shader failed: %d\n", fragmentShader);
      printShaderLog(fragmentShader);
      return false;
    }
  }

  GLuint colorfulFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  {
    glShaderSource(colorfulFragmentShader, 1, &colorfulFragmentShaderSource, NULL);
    glCompileShader(colorfulFragmentShader);

    glGetShaderiv(colorfulFragmentShader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE)
    {
      printf("compile fragment shader failed: %d\n", colorfulFragmentShader);
      printShaderLog(colorfulFragmentShader);
      return false;
    }
  }

  {
    g_shaderPrograms[0] = glCreateProgram();
    glAttachShader(g_shaderPrograms[0], vertexShader);
    glAttachShader(g_shaderPrograms[0], fragmentShader);
    glLinkProgram(g_shaderPrograms[0]);

    glGetProgramiv(g_shaderPrograms[0], GL_LINK_STATUS, &ok);
    if (ok != GL_TRUE)
    {
      printf("link shader program failed: %d\n", g_shaderPrograms[0]);
      printProgramLog(g_shaderPrograms[0]);
      return false;
    }
  }

  {
    g_shaderPrograms[1] = glCreateProgram();
    glAttachShader(g_shaderPrograms[1], vertexShader);
    glAttachShader(g_shaderPrograms[1], colorfulFragmentShader);
    glLinkProgram(g_shaderPrograms[1]);

    glGetProgramiv(g_shaderPrograms[1], GL_LINK_STATUS, &ok);
    if (ok != GL_TRUE)
    {
      printf("link shader program failed: %d\n", g_shaderPrograms[1]);
      printProgramLog(g_shaderPrograms[1]);
      return false;
    }
  }

  // TODO: in case of error, delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(colorfulFragmentShader);

  //  glPolygonMode(GL_FRONT_LEFT, GL_LINE);

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
  glDeleteProgram(g_shaderProgram);

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
