#ifndef OLD_H
#define OLD_H

#include "unity.h"

#include "flycamera.h"
#include "shader.h"

int g_screenWidth = 1000;
int g_screenHeight = 600;

GLint g_vertexPos2DLocation = -1;
GLuint g_vao = 0;
GLuint g_vbo = 0;
GLuint g_ebo = 0;

GLuint g_vao1 = 0;
GLuint g_vbo1 = 0;

shader_s g_shaders[2] = {{0}, {0}};
GLuint g_tex[2] = {0, 0};

Camera g_camera = {};

void initTwoVAO() {
  float triangle1[] = {// first bottom left
                       -1.0f, -1.0f, 0.0f,

                       1.0f, 1.0f, 1.0f,

                       0.0f, 0.0f,

                       // first top left
                       -1.0f, 1.0f, 0.0f,

                       1.0f, 1.0f, 1.0f,

                       0.0f, 2.0f,

                       // first bottom right
                       1.0f, -1.0f, 0.0f,

                       1.0f, 1.0f, 1.0f,

                       2.0f, 0.0f};

  float triangle2[] = {// second top left
                       -1.0f, 1.0f, 0.0f,

                       1.0f, 1.0f, 1.0f,

                       0.0f, 2.0f,

                       // second top right
                       1.0f, 1.0f, 0.0f,

                       1.0f, 1.0f, 1.0f,

                       2.0f, 2.0f,

                       // second bottom right
                       1.0f, -1.0f, 0.0f,

                       1.0f, 1.0f, 1.0f,

                       2.0f, 0.0f};

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);
  glGenBuffers(1, &g_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glGenVertexArrays(1, &g_vao1);
  glBindVertexArray(g_vao1);
  glGenBuffers(1, &g_vbo1);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

void renderTwoVAO() {

  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

  // actually reversed direction vector by convention
  // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
  // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
  // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

  float timeValue = SDL_GetTicks() / 1000.0f;
  float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
  // float x = (sin(timeValue) / 2.0f) + 0.5f;
  // float y = (cos(timeValue) / 2.0f) + 0.5f;

  // timeValue = timeValue + (sin(timeValue) / 2.0f) + 0.5f; // units per second

  float x_max = 0.75f;
  float x_min = -0.5f;
  float x_diff = x_max - x_min;

  float x =
      (fmod(timeValue, 2 * x_diff) < x_diff ? x_min + fmod(timeValue, x_diff)
                                            : x_max - fmod(timeValue, x_diff));

  float y_max = 0.5f;
  float y_min = -1.0f;
  float y_diff = y_max - y_min;

  float y =
      (fmod(timeValue, 2 * y_diff) < y_diff ? y_min + fmod(timeValue, y_diff)
                                            : y_max - fmod(timeValue, y_diff));

  // float y = (fmod(timeValue, 2.0f) <= 1.0f ? -fmod(timeValue, 1.0f) : 1.0f -
  // fmod(timeValue, 1.0f)); printf("x: %f, y: %f\n", x, y);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0, 0, -3.0f));

  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f),
                                (float)g_screenWidth / (float)g_screenHeight,
                                0.1f, 100.0f);

  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0, 1));
  // trans = glm::scale(trans, glm::vec3(sin(timeValue), cos(timeValue), .75f));

  shader_use(&g_shaders[0]);
  shader_4f(&g_shaders[0], "ourColor", 0.0f, greenValue, 0.0f, 1.0f);
  shader_3f(&g_shaders[0], "ourPosition", x, y, 0.0f);
  // can be done only once
  shader_1i(&g_shaders[0], "tex1", 0);
  shader_1i(&g_shaders[0], "tex2", 1);
  shader_mat4fv(&g_shaders[0], "transform", glm::value_ptr(trans));

  shader_mat4fv(&g_shaders[0], "model", glm::value_ptr(model));
  shader_mat4fv(&g_shaders[0], "view", glm::value_ptr(view));
  shader_mat4fv(&g_shaders[0], "projection", glm::value_ptr(projection));

  glBindVertexArray(g_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  shader_use(&g_shaders[1]);
  shader_3f(&g_shaders[1], "ourPosition", x, y, 0.0f);
  // can be done only once
  shader_1i(&g_shaders[1], "tex1", 0);
  shader_1i(&g_shaders[1], "tex2", 1);
  shader_mat4fv(&g_shaders[1], "transform", glm::value_ptr(trans));

  shader_mat4fv(&g_shaders[1], "model", glm::value_ptr(model));
  shader_mat4fv(&g_shaders[1], "view", glm::value_ptr(view));
  shader_mat4fv(&g_shaders[1], "projection", glm::value_ptr(projection));

  glBindVertexArray(g_vao1);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  {
    // good transform order: on the right
    glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.75f, 0.f, 0.0f));
    trans =
        glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0, 1));
    // trans = glm::scale(trans, glm::vec3(fmod(timeValue, 1.f) + 0.5,
    // fmod(timeValue, 1.f) + 0.5, fmod(timeValue, 1.f) + 0.5)); trans =
    // glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0, 1));
    shader_use(&g_shaders[0]);
    shader_4f(&g_shaders[0], "ourColor", 1.0f, 1.f, 1.0f, 1.0f);
    shader_3f(&g_shaders[0], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_1i(&g_shaders[0], "tex1", 0);
    shader_1i(&g_shaders[0], "tex2", 1);
    shader_mat4fv(&g_shaders[0], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    shader_use(&g_shaders[1]);
    shader_3f(&g_shaders[1], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_1i(&g_shaders[1], "tex1", 0);
    shader_1i(&g_shaders[1], "tex2", 1);
    shader_mat4fv(&g_shaders[1], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
  {
    // bad transform order: rotate around the wrong point
    glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::scale(trans, glm::vec3(fmod(timeValue, 1.f) + 0.5,
    // fmod(timeValue, 1.f) + 0.5, fmod(timeValue, 1.f) + 0.5)); second rotate
    // around pivot point
    trans =
        glm::rotate(trans, glm::radians(timeValue * 20), glm::vec3(0, 0, 1));
    // first translate
    trans = glm::translate(trans, glm::vec3(-0.75f, 0.f, 0.0f));
    // trans = glm::rotate(trans, glm::radians(timeValue * 50), glm::vec3(0, 0,
    // 1));
    shader_use(&g_shaders[0]);
    shader_4f(&g_shaders[0], "ourColor", 1.0f, 1.f, 1.0f, 1.0f);
    shader_3f(&g_shaders[0], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_1i(&g_shaders[0], "tex1", 0);
    shader_1i(&g_shaders[0], "tex2", 1);
    shader_mat4fv(&g_shaders[0], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    shader_use(&g_shaders[1]);
    shader_3f(&g_shaders[1], "ourPosition", 0.f, 0.f, 0.0f);
    // can be done only once
    shader_1i(&g_shaders[1], "tex1", 0);
    shader_1i(&g_shaders[1], "tex2", 1);
    shader_mat4fv(&g_shaders[1], "transform", glm::value_ptr(trans));

    glBindVertexArray(g_vao1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}

void triangleExampleVBO() {
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

  float vertices[] = {// first bottom left
                      -.5f, 0.0f, 0.0f,

                      // first top left
                      -.5f, 0.5f, 0.0f,

                      // first bottom right
                      -.25f, 0.0f, 0.0f,

                      // second top left
                      0.0f, 0.5f, 0.0f,

                      // second top right
                      .25f, 0.5f, 0.0f,

                      // second bottom right
                      .25f, 0.0f, 0.0f};

  unsigned int indices[] = {0, 1, 2, 3, 4, 5};

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);

  glGenBuffers(1, &g_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &g_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

void clean() {

  glDeleteVertexArrays(1, &g_vao1);
  glDeleteVertexArrays(1, &g_vao);
  glDeleteBuffers(1, &g_vbo);
  glDeleteBuffers(1, &g_vbo1);
  shader_clean(&g_shaders[0]);
  shader_clean(&g_shaders[1]);
}
#endif