#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>

#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define internal static
#define local_persist static
#define global_variable static

int g_screenWidth = 1000;
int g_screenHeight = 600;
SDL_Window *g_window;
SDL_GLContext g_ctx;

GLint g_vertexPos2DLocation = -1;
GLuint g_vao = 0;
GLuint g_vbo = 0;
GLuint g_ebo = 0;

GLuint g_vao1 = 0;
GLuint g_vbo1 = 0;

shader_s g_shaders[2] = {{0}, {0}};
GLuint g_tex[2] = {0, 0};

void initCube()
{
  float vertices[] = {
      // front
      -0.5f, -0.5f, -0.5f,
      0.0f, 0.0f,


      -0.5f, 0.5f, -0.5f, // Front Left Top
      0.0f, 1.0f,         // Texture

      0.5f, 0.5f, -0.5f, // Front Right Top
      1.0f, 1.0f,        // Texture

      0.5f, -0.5f, -0.5f, // Front Right Bottom
      1.0f, 0.0f,        // Texture


      // right
      0.5f, -0.5f, -0.5f,
      0.0f, 0.0f,

      0.5f, 0.5f, -0.5f,
      1.0f, 0.0f,

      0.5f, 0.5f, 0.5f,
      1.0f, 1.0f,

      0.5f, -0.5f, 0.5f,
      0.0f, 1.0f,

      // back
      0.5f, -0.5f, 0.5f,
      0.0f, 0.0f,

      0.5f, 0.5f, 0.5f,
      0.0f, 1.0f,

      -0.5f, 0.5f, 0.5f,
      1.0f, 1.0f,

      -0.5f, -0.5f, 0.5f,
      1.0f, 0.0f,

      // left
      -0.5f, -0.5f, 0.5f,
      0.0f, 0.0f,

      -0.5f, 0.5f, 0.5f,
      0.0f, 1.0f,

      -0.5f, 0.5f, -0.5f,
      1.0f, 1.0f,

      -0.5f, -0.5f, -0.5f,
      1.0f, 0.0f,

      // top

      -0.5f, 0.5f, -0.5f,
      0.0f, 0.0f,

      -0.5, 0.5f, 0.5f,
      0.0f, 1.0f,

      0.5f, 0.5f, 0.5f,
      1.0f, 1.01f,

      0.5, 0.5f, -0.5f,
      1.0f, 0.0f,

// bottom
      -0.5f, -0.5f, -0.5f,
      0.0f, 0.0f,

      -0.5, -0.5f, 0.5f,
      0.0f, 1.0f,

      0.5f, -0.5f, 0.5f,
      1.0f, 1.01f,

      0.5, -0.5f, -0.5f,
      1.0f, 0.0f






      // -0.5f, -0.5f, 0.5f, // Back Left Bottom
      // 1.0f, 1.0f, 1.0f,   // Color
      // 1.0f, 1.0f,         // Texture

      // -0.5f, 0.5f, 0.5f, // Back Left Top
      // 1.0f, 1.0f, 1.0f,  // Color
      // 1.0f, 0.0f,        // Texture

      // 0.5f, 0.5f, 0.5f, // Back Right Top
      // 1.0f, 1.0f, 1.0f, // Color
      // 0.0f, 0.0f,       // Texture

      // 0.5f, -0.5f, 0.5f, // Back Right Bottom
      // 1.0f, 1.0f, 1.0f, // Color
      // 0.0f, 1.0f,       // Texture
  };

  unsigned int faces[] = {
      0, 1, 2, // Front

      2, 3, 0,

      4,5,6,
      6,7,4,

      8,9,10,
      10,11,8,

      12,13,14,
      14,15,12,

      16, 17, 18,
      18, 19, 16,

      20, 21, 22,
      22, 23, 20,





  //     // 4, 5, 6, // Back
  //     // 6, 7,4,

  //     // 0, 1, 5, // Left
  //     // 5, 4, 0,

  //     // 3, 2, 6, // Right
  //     // 6, 7, 3,

  //     // 1, 2, 6, // Top
  //     // 6, 5, 1,

  //     // 0, 3, 7, // Bottom
  //     // 7, 4, 0,
  };

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);
  glGenBuffers(1, &g_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &g_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
}

void renderCube()
{
  float timeValue = SDL_GetTicks() / 1000.0f;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(20.0f * timeValue), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0, 0, -3.0f));
  view = glm::rotate(view, glm::radians(10 * timeValue), glm::vec3(0.0, 1.0, 1.0));

  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), (float)g_screenWidth / (float)g_screenHeight, 0.1f, 100.0f);


  glUseProgram(g_shaders[1].program);

  shader_Set1i(&g_shaders[1], "tex1", 0);
  shader_Set1i(&g_shaders[1], "tex2", 1);
  shader_SetMatrix4fv(&g_shaders[1], "model", glm::value_ptr(model));
  shader_SetMatrix4fv(&g_shaders[1], "view", glm::value_ptr(view));
  shader_SetMatrix4fv(&g_shaders[1], "projection", glm::value_ptr(projection));


  glBindVertexArray(g_vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glm::vec3 positions[] = {
    glm::vec3(0.2f, -0.2f, 1.0f),
    glm::vec3(-0.4f, 0.4f, 0.5f),
    glm::vec3(0.0f, 0.0f, 1.5f),
    glm::vec3(0.89f, 0.5f, 2.0f),
  };

  for (int i = 0; i < 3; i++ ) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, positions[i]);
    model = glm::rotate(model, glm::radians(timeValue * 25 * (i+1)), positions[i]);

    shader_SetMatrix4fv(&g_shaders[1], "model", glm::value_ptr(model));
    glBindVertexArray(g_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
  // glDrawArrays(GL_TRIANGLES, 0, 36);
}
void initTwoVAO()
{
  float triangle1[] =
  {
      // first bottom left
      -1.0f,
      -1.0f,
      0.0f,

      1.0f,
      1.0f,
      1.0f,

      0.0f,
      0.0f,

      // first top left
      -1.0f,
      1.0f,
      0.0f,

      1.0f,
      1.0f,
      1.0f,

      0.0f,
      2.0f,

      // first bottom right
      1.0f,
      -1.0f,
      0.0f,

      1.0f,
      1.0f,
      1.0f,

      2.0f,
      0.0f};

  float triangle2[] = {
      // second top left
      -1.0f,
      1.0f,
      0.0f,

      1.0f,
      1.0f,
      1.0f,

      0.0f,
      2.0f,

      // second top right
      1.0f,
      1.0f,
      0.0f,

      1.0f,
      1.0f,
      1.0f,

      2.0f,
      2.0f,

      // second bottom right
      1.0f,
      -1.0f,
      0.0f,

      1.0f,
      1.0f,
      1.0f,

      2.0f,
      0.0f};

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);
  glGenBuffers(1, &g_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glGenVertexArrays(1, &g_vao1);
  glBindVertexArray(g_vao1);
  glGenBuffers(1, &g_vbo1);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

void renderTwoVAO()
{

  float timeValue = SDL_GetTicks() / 1000.0f;
  float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
  // float x = (sin(timeValue) / 2.0f) + 0.5f;
  // float y = (cos(timeValue) / 2.0f) + 0.5f;

  // timeValue = timeValue + (sin(timeValue) / 2.0f) + 0.5f; // units per second

  float x_max = 0.75f;
  float x_min = -0.5f;
  float x_diff = x_max - x_min;

  float x = (fmod(timeValue, 2 * x_diff) < x_diff ? x_min + fmod(timeValue, x_diff) : x_max - fmod(timeValue, x_diff));

  float y_max = 0.5f;
  float y_min = -1.0f;
  float y_diff = y_max - y_min;

  float y = (fmod(timeValue, 2 * y_diff) < y_diff ? y_min + fmod(timeValue, y_diff) : y_max - fmod(timeValue, y_diff));

  // float y = (fmod(timeValue, 2.0f) <= 1.0f ? -fmod(timeValue, 1.0f) : 1.0f - fmod(timeValue, 1.0f));
  // printf("x: %f, y: %f\n", x, y);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0, 0, -3.0f));

  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), (float)g_screenWidth / (float)g_screenHeight, 0.1f, 100.0f);

  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0, 1));
  // trans = glm::scale(trans, glm::vec3(sin(timeValue), cos(timeValue), .75f));

  shader_Use(&g_shaders[0]);
  shader_Set4f(&g_shaders[0], "ourColor", 0.0f, greenValue, 0.0f, 1.0f);
  shader_Set3f(&g_shaders[0], "ourPosition", x, y, 0.0f);
  // can be done only once
  shader_Set1i(&g_shaders[0], "tex1", 0);
  shader_Set1i(&g_shaders[0], "tex2", 1);
  shader_SetMatrix4fv(&g_shaders[0], "transform", glm::value_ptr(trans));

  shader_SetMatrix4fv(&g_shaders[0], "model", glm::value_ptr(model));
  shader_SetMatrix4fv(&g_shaders[0], "view", glm::value_ptr(view));
  shader_SetMatrix4fv(&g_shaders[0], "projection", glm::value_ptr(projection));

  glBindVertexArray(g_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  shader_Use(&g_shaders[1]);
  shader_Set3f(&g_shaders[1], "ourPosition", x, y, 0.0f);
  // can be done only once
  shader_Set1i(&g_shaders[1], "tex1", 0);
  shader_Set1i(&g_shaders[1], "tex2", 1);
  shader_SetMatrix4fv(&g_shaders[1], "transform", glm::value_ptr(trans));

  shader_SetMatrix4fv(&g_shaders[1], "model", glm::value_ptr(model));
  shader_SetMatrix4fv(&g_shaders[1], "view", glm::value_ptr(view));
  shader_SetMatrix4fv(&g_shaders[1], "projection", glm::value_ptr(projection));

  glBindVertexArray(g_vao1);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  {
    // good transform order: on the right
    glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.75f, 0.f, 0.0f));
    trans = glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0, 1));
    // trans = glm::scale(trans, glm::vec3(fmod(timeValue, 1.f) + 0.5, fmod(timeValue, 1.f) + 0.5, fmod(timeValue, 1.f) + 0.5));
    // trans = glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0, 1));
    shader_Use(&g_shaders[0]);
    shader_Set4f(&g_shaders[0], "ourColor", 1.0f, 1.f, 1.0f, 1.0f);
    shader_Set3f(&g_shaders[0], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_Set1i(&g_shaders[0], "tex1", 0);
    shader_Set1i(&g_shaders[0], "tex2", 1);
    shader_SetMatrix4fv(&g_shaders[0], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    shader_Use(&g_shaders[1]);
    shader_Set3f(&g_shaders[1], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_Set1i(&g_shaders[1], "tex1", 0);
    shader_Set1i(&g_shaders[1], "tex2", 1);
    shader_SetMatrix4fv(&g_shaders[1], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
  {
    // bad transform order: rotate around the wrong point
    glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::scale(trans, glm::vec3(fmod(timeValue, 1.f) + 0.5, fmod(timeValue, 1.f) + 0.5, fmod(timeValue, 1.f) + 0.5));
    // second rotate around pivot point
    trans = glm::rotate(trans, glm::radians(timeValue * 20), glm::vec3(0, 0, 1));
    // first translate
    trans = glm::translate(trans, glm::vec3(-0.75f, 0.f, 0.0f));
    // trans = glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0, 1));
    shader_Use(&g_shaders[0]);
    shader_Set4f(&g_shaders[0], "ourColor", 1.0f, 1.f, 1.0f, 1.0f);
    shader_Set3f(&g_shaders[0], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_Set1i(&g_shaders[0], "tex1", 0);
    shader_Set1i(&g_shaders[0], "tex2", 1);
    shader_SetMatrix4fv(&g_shaders[0], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    shader_Use(&g_shaders[1]);
    shader_Set3f(&g_shaders[1], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_Set1i(&g_shaders[1], "tex1", 0);
    shader_Set1i(&g_shaders[1], "tex2", 1);
    shader_SetMatrix4fv(&g_shaders[1], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}

internal void render()
{
  glViewport(0, 0, g_screenWidth, g_screenHeight);
  glClearColor(0.15625f,0.15625f,0.15625f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // renderTwoVAO();
  renderCube();
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

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  return true;
}

internal bool
initTexture()
{
  {
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./assets/wall.jpg", &width, &height, &nrChannels, 0);
    if (!data)
    {
      printf("Failed to load texture");
      return false;
    }

    glGenTextures(1, &g_tex[0]);
    glBindTexture(GL_TEXTURE_2D, g_tex[0]);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float borderColor[] = {1.0f, 0.2f, 0.45f, 0.5f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }
  {
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (!data)
    {
      printf("Failed to load texture");
      return false;
    }

    glGenTextures(1, &g_tex[1]);
    glBindTexture(GL_TEXTURE_2D, g_tex[1]);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }

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
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

  glEnable(GL_DEPTH_TEST);

  // initTwoVAO();
  initCube();

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  printf("Maximum nr of vertex attributes supported: %d\n", nrAttributes);
;
  if (!initTexture())
  {
    printf("init texture failed\n");
    return false;
  }

  // has to be done once
  // need activated program
  // shader_Use(&g_shaders[0]);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, g_tex[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, g_tex[1]);

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

      switch (e.type) {
        case SDL_WINDOWEVENT:
        if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
          g_screenWidth = e.window.data1;
          g_screenHeight = e.window.data2;
        }
      }
    }

    render();
  }

  clean();

  return 0;
}
