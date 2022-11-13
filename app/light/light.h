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

void app_maze_render(app_s* app, float dt) {
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

    float cell_width = 2.0f;
    float cell_height = 4.3f;
    float cell_depth = 2.3f;

    float cell_fluctuation = 0.5f;
    static int save_rnd = rnd;
    rnd = save_rnd;

    for (int l = 0; l < 3; l++) {

      for (int i = 0; i < g_maze_size; i++) {
        for (int j = 0; j < g_maze_size; j++) {
          if (g_maze[l * g_maze_size * g_maze_size + i * g_maze_size + j] ==
              1) {

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, -2.0f, 2.0f));
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

void app_scene_mat_view_render(app_s* app, float dt) {
  flycamera_update(&app->camera, dt);

  float time = SDL_GetTicks() / 1000.0f;

  float max_x = 5.0f;
  float max_y = 4.0f;
  time /= 2.3f;
  time = fmod(time, 4.0f);

  glm::vec3 offset(0.0f);
  offset.z = -1.0f;
  if (time < 1.0f) {
    offset.x = time * max_x;
  } else if (time < 2.0f) {
    time -= 1.0f;
    offset.x = max_x - time * max_x;
    offset.y = time * max_y;
  } else if (time < 3.0f) {
    time -= 2.0f;
    offset.x = time * max_x;
    offset.y = max_y;
  } else if (time < 4.0f) {
    time -= 3.0f;
    offset.x = max_x - time * max_x;
    offset.y = max_y - time * max_y;
  }

  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 light_pos = offset;


  glm::mat4 view = flycamera_get_view_matrix(&app->camera);
  glm::mat4 proj = flycamera_get_projection_matrix(&app->camera);

  // glm::vec3 light_color((sin(time * 2.0f) + 1.0f) / 2.0f,
  //                       (sin(time * 0.7f) + 1.0f) / 2.0f,
  //                       (sin(time * 1.3f) + 1.0f) / 2.0f);
  glm::vec3 light_color(1.0f);

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

  static material_s emerald = {
      .ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f),
      .diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f),
      .specular = glm::vec3(0.633f, 0.727811f, 0.633f),
      .shininess = 0.6f * 128.0f,
  };

  static material_s jade = {
      .ambient = glm::vec3(0.135f, 0.2225f, 0.1575f),
      .diffuse = glm::vec3(0.54f, 0.89f, 0.63f),
      .specular = glm::vec3(0.316228f, 0.316228f, 0.316228f),
      .shininess = 0.1f * 128.0f,
  };

  static material_s obsidian = {
      .ambient = glm::vec3(0.05375f, 0.05f, 0.06625f),
      .diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f),
      .specular = glm::vec3(0.332741f, 0.328634f, 0.346435f),
      .shininess = 0.3f * 128.0f,
  };

  static material_s pearl = {
      .ambient = glm::vec3(0.25f, 0.20725f, 0.20725f),
      .diffuse = glm::vec3(1.0f, 0.829f, 0.829f),
      .specular = glm::vec3(0.296648f, 0.296648f, 0.296648f),
      .shininess = 0.088f * 128.0f,
  };
  static material_s ruby = {
      .ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f),
      .diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f),
      .specular = glm::vec3(0.727811f, 0.626959f, 0.626959f),
      .shininess = 0.6f * 128.0f,
  };

  static material_s turquoise = {
      .ambient = glm::vec3(0.1f, 0.18725f, 0.1745f),
      .diffuse = glm::vec3(0.396f, 0.74151f, 0.69102f),
      .specular = glm::vec3(0.297254f, 0.30829f, 0.306678f),
      .shininess = 0.1f * 128.0f,
  };
  static material_s brass = {
      .ambient = glm::vec3(0.329412f, 0.223529f, 0.027451f),
      .diffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f),
      .specular = glm::vec3(0.992157f, 0.941176f, 0.807843f),
      .shininess = 0.21794872f * 128.0f,
  };

  static material_s bronze = {
      .ambient = glm::vec3(0.2125f, 0.1275f, 0.054f),
      .diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f),
      .specular = glm::vec3(0.393548f, 0.271906f, 0.166721f),
      .shininess = 0.2f * 128.0f,
  };

  static material_s chrome = {
      .ambient = glm::vec3(0.25f, 0.25f, 0.25f),
      .diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
      .specular = glm::vec3(0.774597f, 0.774597f, 0.774597f),
      .shininess = 0.6f * 128.0f,
  };

  static material_s copper = {
      .ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f),
      .diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f),
      .specular = glm::vec3(0.256777f, 0.137622f, 0.086014f),
      .shininess = 0.1f * 128.0f,
  };

  static material_s gold = {
      .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
      .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f),
      .specular = glm::vec3(0.628281f, 0.555802f, 0.366065f),
      .shininess = 0.4f * 128.0f,
  };

  static material_s silver = {
      .ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f),
      .diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f),
      .specular = glm::vec3(0.508273f, 0.508273f, 0.508273f),
      .shininess = 0.4f * 128.0f,
  };

  static material_s black_plastic = {
      .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
      .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
      .specular = glm::vec3(0.50f, 0.50f, 0.50f),
      .shininess = 0.25f * 128.0f,
  };

  static material_s cyan_plastic = {
      .ambient = glm::vec3(0.0f, 0.1f, 0.06f),
      .diffuse = glm::vec3(0.0f, 0.50980392f, 0.50980392f),
      .specular = glm::vec3(0.50196078f, 0.50196078f, 0.50196078f),
      .shininess = 0.25f * 128.0f,
  };

  static material_s green_plastic = {
      .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
      .diffuse = glm::vec3(0.1f, 0.35f, 0.1f),
      .specular = glm::vec3(0.45f, 0.55f, 0.45f),
      .shininess = 0.25f * 128.0f,
  };

  static material_s red_plastic = {
      .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
      .diffuse = glm::vec3(0.5f, 0.0f, 0.0f),
      .specular = glm::vec3(0.7f, 0.6f, 0.6f),
      .shininess = 0.25f * 128.0f,
  };

  static material_s white_plastic = {
      .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
      .diffuse = glm::vec3(0.55f, 0.55f, 0.55f),
      .specular = glm::vec3(0.70f, 0.70f, 0.70f),
      .shininess = 0.25f * 128.0f,
  };
  static material_s yellow_plastic = {
      .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
      .diffuse = glm::vec3(0.5f, 0.5f, 0.0f),
      .specular = glm::vec3(0.60f, 0.60f, 0.50f),
      .shininess = 0.25f * 128.0f,
  };
  static material_s black_rubber = {
      .ambient = glm::vec3(0.02f, 0.02f, 0.02f),
      .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
      .specular = glm::vec3(0.4f, 0.4f, 0.4f),
      .shininess = 0.078125f * 128.0f,
  };
  static material_s cyan_rubber = {
      .ambient = glm::vec3(0.0f, 0.05f, 0.05f),
      .diffuse = glm::vec3(0.4f, 0.5f, 0.5f),
      .specular = glm::vec3(0.04f, 0.7f, 0.7f),
      .shininess = 0.078125f * 128.0f,
  };
  static material_s green_rubber = {
      .ambient = glm::vec3(0.0f, 0.05f, 0.0f),
      .diffuse = glm::vec3(0.4f, 0.5f, 0.4f),
      .specular = glm::vec3(0.04f, 0.7f, 0.04f),
      .shininess = 0.078125f * 128.0f,
  };
  static material_s red_rubber = {
      .ambient = glm::vec3(0.05f, 0.0f, 0.0f),
      .diffuse = glm::vec3(0.5f, 0.4f, 0.4f),
      .specular = glm::vec3(0.7f, 0.04f, 0.04f),
      .shininess = 0.078125f * 128.0f,
  };

  static material_s white_rubber = {
      .ambient = glm::vec3(0.05f, 0.05f, 0.05f),
      .diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
      .specular = glm::vec3(0.7f, 0.7f, 0.7f),
      .shininess = 0.078125f * 128.0f,
  };


  static material_s yellow_rubber = {
      .ambient = glm::vec3(0.05f, 0.05f, 0.0f),
      .diffuse = glm::vec3(0.5f, 0.5f, 0.4f),
      .specular = glm::vec3(0.7f, 0.7f, 0.04f),
      .shininess = 0.078125f * 128.0f,
  };

  static material_s mat[] = {
      emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze, chrome,
      copper, gold, silver, black_plastic, cyan_plastic, green_plastic,
      red_plastic, white_plastic, yellow_plastic, black_rubber, cyan_rubber,
      green_rubber, red_rubber, white_rubber, yellow_rubber,
  };

  #define NUM_MATERIALS (sizeof(mat) / sizeof(mat[0]))

  int columns = 6;

  for (int i = 0; i < NUM_MATERIALS; i++) {
    int row = i / columns;
    int col = i % columns;
    glm::vec3 pos = glm::vec3(col * 1.0f, row * 1.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

    mr_render(&g_cube, mat[i], model, view, proj, camera_view_pos, &g_light);
  }
}


void app_update(app_s* app, float dt) { app_scene_mat_view_render(app, dt); }

#endif