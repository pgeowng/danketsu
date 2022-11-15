#ifndef LIGHT_H
#define LIGHT_H

#include "debug.h"
#include "flycamera.h"
#include "shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>

#include "mesh_renderer.h"

#define MAZE_SIZE 10

int g_maze[MAZE_SIZE * MAZE_SIZE * 3] = { 0 };
int g_maze_size = MAZE_SIZE;

int rnd = 41241515;
int rnd_mod = 489414;
int rnd_step = 12345;

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
  ok = shader_init(&app->lighting_shader, "./app/light/light.vert",
                   "./app/light/light.frag");
  if (!ok) {
    printf("lighting shader new failed");
    return ok;
  }

  ok = shader_init(&app->lamp_shader, "./app/light/light.vert",
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
    -x_45deg,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    // up right
    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

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

  g_cube.shader = &app->lighting_shader;
  g_cube.vao = app->vao;
  g_cube.vbo = app->vbo;
  g_cube.num_triangles = 72;

  g_ramp.shader = &app->lighting_shader;
  g_ramp.vao = app->ramp_vao;
  g_ramp.vbo = app->ramp_vbo;
  g_ramp.num_triangles = 24;

  for (int l = 0; l < 3; l++) {

    for (int i = 0; i < g_maze_size; i++) {
      for (int j = 0; j < g_maze_size; j++) {
        g_maze[l * g_maze_size * g_maze_size + i * g_maze_size + j] =
            (rnd > 60 ? 1 : 0);

        rnd = (rnd * rnd_step) % rnd_mod;
      }
    }
  }

  return ok;
}

void app_update(app_s* app, float dt) {
  flycamera_update(&app->camera, dt);

  float time = SDL_GetTicks() / 1000.0f;

  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 light_pos = glm::vec3(1.2f + cos(time), 1.0f, 2.0f + sin(time));

  glm::mat4 view = flycamera_get_view_matrix(&app->camera);
  glm::mat4 proj = flycamera_get_projection_matrix(&app->camera);

  glm::vec3 light_color((sin(time * 2.0f) + 1.0f) / 2.0f,
                        (sin(time * 0.7f) + 1.0f) / 2.0f,
                        (sin(time * 1.3f) + 1.0f) / 2.0f);

  {
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(0.2f));

    shader_use(&app->lamp_shader);
    shader_3f(&app->lamp_shader, "lightColor", light_color.r, light_color.g,
              light_color.b);
    shader_mat4fv(&app->lamp_shader, "model", glm::value_ptr(model));
    shader_mat4fv(&app->lamp_shader, "view", glm::value_ptr(view));
    shader_mat4fv(&app->lamp_shader, "projection", glm::value_ptr(proj));

    glBindVertexArray(app->light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glDrawArrays(GL_TRIANGLES, 0, 72);
  }

  g_light.diffuse = light_color * glm::vec3(0.5f);
  g_light.ambient = g_light.diffuse * glm::vec3(0.2f);
  // g_light.specular =
  //     glm::vec3((light_color.r + light_color.g + light_color.b) / 3.0f);
  g_light.specular = glm::vec3(glm::compMax(light_color));

  glm::vec3 light_view_pos = glm::vec3(view * glm::vec4(light_pos, 1.0f));
  g_light.position = light_view_pos;

  glm::vec3 camera_view_pos =
      glm::vec3(view * glm::vec4(app->camera.position, 1.0f));

  {
    model = glm::mat4(1.0f);
    mr_render(&g_cube, g_mat0, model, view, proj, camera_view_pos, &g_light);
  }

  {
    model = glm::mat4(1.0f);
    float time = SDL_GetTicks() / 1000.0f;
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
    model = glm::rotate(model, glm::radians(SDL_GetTicks() / 1000.0f * 50.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(cos(time / 2.0f) + 2.0f,
                                        sin(time / 3.0f) + 1.3f, 1.0f));

    mr_render(&g_ramp, g_mat2, model, view, proj, camera_view_pos, &g_light);
  }

  {
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 2.0f));
    float animation_duration = 3.0f;
    // forward and reverse mode
    float animation_period = animation_duration * 2.0f;

    float animation_time = fmod(time, animation_period) / animation_duration;

    // reverse mode
    if (animation_time > 1.0f) {
      animation_time = 2.0f - animation_time;
    }

    model = glm::rotate(model, glm::radians(3.1415f * animation_time),
                        glm::vec3(0.0f, 1.0f, 0.0f));

    mr_render(&g_ramp, g_mat2, model, view, proj, camera_view_pos, &g_light);
  }

  {

    float cell_width = 2.0f / 2;
    float cell_height = 4.3f / 2;
    float cell_depth = 2.3f / 2;

    float cell_fluctuation = 0.5f;
    static int save_rnd = rnd;
    rnd = save_rnd;

    for (int l = 0; l < 3; l++) {

      for (int i = 0; i < g_maze_size; i++) {
        for (int j = 0; j < g_maze_size; j++) {
          if (g_maze[l * g_maze_size * g_maze_size + i * g_maze_size + j] ==
              1) {

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, -5.0f, 2.0f));
            model = glm::translate(
                model,
                glm::vec3(i * cell_width - g_maze_size / 2.0f, l * cell_height,
                          j * cell_depth - g_maze_size / 2.0f));

            model = glm::scale(model, glm::vec3(cell_width - cell_fluctuation,
                                                cell_height - cell_fluctuation,
                                                cell_depth - cell_fluctuation));

            model = glm::translate(
                model,
                glm::vec3(cell_fluctuation *
                              float(rnd = (rnd * rnd_step) % rnd_mod) / rnd_mod,
                          cell_fluctuation *
                              float(rnd = (rnd * rnd_step) % rnd_mod) / rnd_mod,
                          cell_fluctuation *
                              float(rnd = (rnd * rnd_step) % rnd_mod) /
                              rnd_mod));

            ;

            mr_render(&g_cube, g_mat1, model, view, proj, camera_view_pos,
                      &g_light);
          }
        }
      }
    }
  }
}

#endif