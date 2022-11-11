#ifndef LIGHT_H
#define LIGHT_H

#include "debug.h"
#include "flycamera.h"
#include "shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct app_s {
  // glm::vec3 position;
  // glm::vec3 color;
  // glm::vec3 ambient;
  // glm::vec3 diffuse;
  // glm::vec3 specular;

  GLuint vao;
  GLuint light_vao;
  GLuint vbo;
  GLuint ramp_vao;
  GLuint ramp_vbo;
  GLuint ebo;

  flycamera_s camera = {};
  shader_s lighting_shader = {};
  shader_s lamp_shader = {};
};

bool app_init(app_s* app) {
  bool ok = false;
  flycamera_init(&app->camera, false);
  ok = shader_New(&app->lighting_shader, "./app/light/light.vert",
                  "./app/light/light.frag");
  if (!ok) {
    printf("lighting shader new failed");
    return ok;
  }

  ok = shader_New(&app->lamp_shader, "./app/light/light.vert",
                  "./app/light/light_src.frag");
  if (!ok) {
    printf("lamp shader new failed");
    return ok;
  }

  float cube[] = { // front
                   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f,

                   -0.5f, 0.5f, -0.5f, // Front Left Top
                   0.0f, 0.0f, -0.5f,

                   0.5f, 0.5f, -0.5f, // Front Right Top
                   0.0f, 0.0f, -0.5f,

                   0.5f, 0.5f, -0.5f, // Front Right Top
                   0.0f, 0.0f, -0.5f,

                   0.5f, -0.5f, -0.5f, // Front Right Bottom
                   0.0f, 0.0f, -0.5f,

                   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f,

                   // right
                   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

                   0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

                   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                   0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

                   // back
                   0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                   0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                   -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                   -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                   -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                   0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                   // left
                   -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

                   -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

                   -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

                   -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

                   -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

                   -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

                   // top
                   -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

                   -0.5, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

                   0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

                   0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

                   0.5, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

                   -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

                   // bottom
                   -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

                   -0.5, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,

                   0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,

                   0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,

                   0.5, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

                   -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f
  };

  float x_45deg = 0.70710678118f;

  float ramp[] = {
    // up
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    x_45deg,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    x_45deg,

    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    x_45deg,

    // up right
    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    x_45deg,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    x_45deg,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    x_45deg,

    // left
    -0.5f,
    -0.5f,
    -0.5f,
    -1.0f,
    0.0f,
    0.0f,

    -0.5f,
    -0.5f,
    0.5f,
    -1.0f,
    0.0f,
    0.0f,

    -0.5f,
    0.5f,
    0.5f,
    -1.0f,
    0.0f,
    0.0f,

    // right
    0.5f,
    -0.5f,
    -0.5f,
    1.0f,
    0.0f,
    0.0f,

    0.5f,
    -0.5f,
    0.5f,
    1.0f,
    0.0f,
    0.0f,

    0.5f,
    0.5f,
    0.5f,
    1.0f,
    0.0f,
    0.0f,

    // bottom
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,

    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,

    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,

    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,

    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,

    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,

    // back
    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,
  };

  glGenVertexArrays(1, &app->vao);
  glBindVertexArray(app->vao);
  glGenBuffers(1, &app->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenVertexArrays(1, &app->ramp_vao);
  glBindVertexArray(app->ramp_vao);
  glGenBuffers(1, &app->ramp_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, app->ramp_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ramp), ramp, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenVertexArrays(1, &app->light_vao);
  glBindVertexArray(app->light_vao);

  glBindBuffer(GL_ARRAY_BUFFER, app->vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  return ok;
}

void app_update(app_s* app, float dt) {
  flycamera_update(&app->camera, dt);

  float time = SDL_GetTicks() / 1000.0f;

  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 light_pos = glm::vec3(1.2f + cos(time), 1.0f, 2.0f + sin(time));

  glm::mat4 view = flycamera_get_view_matrix(&app->camera);
  glm::mat4 proj = flycamera_get_projection_matrix(&app->camera);

  {
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(0.2f));

    shader_Use(&app->lamp_shader);
    shader_SetMatrix4fv(&app->lamp_shader, "model", glm::value_ptr(model));
    shader_SetMatrix4fv(&app->lamp_shader, "view", glm::value_ptr(view));
    shader_SetMatrix4fv(&app->lamp_shader, "projection", glm::value_ptr(proj));

    glBindVertexArray(app->light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glDrawArrays(GL_TRIANGLES, 0, 72);
  }

  float lr = 91.0f / 256.0f;
  float lg = 194.0f / 256.0f;
  float lb = 102.0f / 256.0f;

  glm::vec3 light_view_pos = glm::vec3(view * glm::vec4(light_pos, 1.0f));
  glm::vec3 camera_view_pos =
      glm::vec3(view * glm::vec4(app->camera.position, 1.0f));

  {
    model = glm::mat4(1.0f);
    shader_Use(&app->lighting_shader);
    shader_Set3f(&app->lighting_shader, "objectColor", 1.0f, 0.5f, 0.31f);
    shader_Set3f(&app->lighting_shader, "lightColor", lr, lg, lb);
    shader_Set3f(&app->lighting_shader, "lightPos", light_view_pos.x,
                 light_view_pos.y, light_view_pos.z);
    shader_Set3f(&app->lighting_shader, "viewPos", camera_view_pos.x,
                 camera_view_pos.y, camera_view_pos.z);

    shader_SetMatrix4fv(&app->lighting_shader, "model", glm::value_ptr(model));
    shader_SetMatrix4fv(&app->lighting_shader, "view", glm::value_ptr(view));
    shader_SetMatrix4fv(&app->lighting_shader, "projection",
                        glm::value_ptr(proj));

    glBindVertexArray(app->vao);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glDrawArrays(GL_TRIANGLES, 0, 72);
  }

  {
    model = glm::mat4(1.0f);
    float time = SDL_GetTicks() / 1000.0f;
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
    model = glm::rotate(model, glm::radians(SDL_GetTicks() / 1000.0f * 50.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(cos(time / 2.0f) + 2.0f,
                                        sin(time / 3.0f) + 1.3f, 1.0f));

    shader_Use(&app->lighting_shader);
    shader_Set3f(&app->lighting_shader, "objectColor", 0.5f, 0.5f, 0.5f);
    shader_Set3f(&app->lighting_shader, "lightColor", lr, lg, lb);
    shader_Set3f(&app->lighting_shader, "lightPos", light_view_pos.x,
                 light_view_pos.y, light_view_pos.z);
    shader_Set3f(&app->lighting_shader, "viewPos", camera_view_pos.x,
                 camera_view_pos.y, camera_view_pos.z);

    shader_SetMatrix4fv(&app->lighting_shader, "model", glm::value_ptr(model));
    shader_SetMatrix4fv(&app->lighting_shader, "view", glm::value_ptr(view));
    shader_SetMatrix4fv(&app->lighting_shader, "projection",
                        glm::value_ptr(proj));

    glBindVertexArray(app->ramp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, app->ramp_vbo);
    glDrawArrays(GL_TRIANGLES, 0, 24);
  }
}

#endif