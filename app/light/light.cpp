#include "light.h"

#define MAZE_SIZE 10

int g_maze[MAZE_SIZE * MAZE_SIZE * 3] = { 0 };
int g_maze_size = MAZE_SIZE;

int rnd = 41241515;
int rnd_mod = 489414;
int rnd_step = 12345;

bool app_init(app_s* app) {
  bool ok = false;
  flycamera_init(&app->camera, false);
  ok = shader_init(&app->lighting_shader, "./app/light/light.vert",
                   "./app/light/light_tex.frag");
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
                   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,

                   -0.5f, 0.5f, -0.5f, // Front Left Top
                   0.0f, 0.0f, -0.5f, 0.0f, 1.0f,

                   0.5f, 0.5f, -0.5f, // Front Right Top
                   0.0f, 0.0f, -0.5f, 1.0f, 1.0f,

                   0.5f, 0.5f, -0.5f, // Front Right Top
                   0.0f, 0.0f, -0.5f, 1.0f, 1.0f,

                   0.5f, -0.5f, -0.5f, // Front Right Bottom
                   0.0f, 0.0f, -0.5f, 1.0f, 0.0f,

                   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,

                   // right
                   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                   0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

                   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                   0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                   // back
                   0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                   0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                   -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                   -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                   -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

                   0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                   // left
                   -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                   -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

                   -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                   -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                   -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                   -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                   // top
                   -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                   -0.5, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

                   0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                   0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                   0.5, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

                   -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                   // bottom
                   -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

                   -0.5, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                   0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

                   0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

                   0.5, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

                   -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

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

  app->mat_tex = {
    .tex_diffuse = 0,
    .tex_specular = 0,
    .tex_diffuse_unit = GL_TEXTURE1,
    .tex_specular_unit = GL_TEXTURE2,
    .tex_diffuse_unit_idx = 1,
    .tex_specular_unit_idx = 2,
    .shininess = 32.0f,
  };

  ok = tex_load(&app->mat_tex.tex_diffuse, "assets/container2.png");
  if (!ok) {
    return ok;
  }

  glActiveTexture(app->mat_tex.tex_diffuse_unit);
  glBindTexture(GL_TEXTURE_2D, app->mat_tex.tex_diffuse);

  ok = tex_load(&app->mat_tex.tex_specular, "assets/container2_specular.png");
  if (!ok) {
    return ok;
  }

  glActiveTexture(app->mat_tex.tex_specular_unit);
  glBindTexture(GL_TEXTURE_2D, app->mat_tex.tex_specular);

  printf("mat tex: %f %d %d %d %d", app->mat_tex.shininess,
         app->mat_tex.tex_diffuse, app->mat_tex.tex_diffuse_unit,
         app->mat_tex.tex_specular, app->mat_tex.tex_specular_unit);

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

  shader_use(&app->lighting_shader);
  shader_1i(&app->lighting_shader, "material.diffuse", 0);
  shader_1i(&app->lighting_shader, "material.specular", 1);

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
    app->mat_color = g_mat_sh_0;
    app_render_mat_color_cube(app, &g_cube, model, view, proj, camera_view_pos,
                              &g_light);
  }
  {
    model = glm::mat4(1.0f);
    float time = SDL_GetTicks() / 1000.0f;
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
    model = glm::rotate(model, glm::radians(SDL_GetTicks() / 1000.0f * 50.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(cos(time / 2.0f) + 2.0f,
                                        sin(time / 3.0f) + 1.3f, 1.0f));

    app->mat_color = g_mat_sh_2;
    app_render_mat_color_cube(app, &g_ramp, model, view, proj, camera_view_pos,
                              &g_light);
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

    app->mat_color = g_mat_sh_2;
    app_render_mat_color_cube(app, &g_ramp, model, view, proj, camera_view_pos,
                              &g_light);
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
            app->mat_color = g_mat_sh_1;
            app_render_mat_color_cube(app, &g_cube, model, view, proj,
                                      camera_view_pos, &g_light);
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

  int columns = 6;

  for (int i = 0; i < COUNT_OF(g_mat_color_materials); i++) {
    int row = i / columns;
    int col = i % columns;
    glm::vec3 pos = glm::vec3(col * 1.0f, row * 1.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

    app->mat_color = g_mat_color_materials[i];
    app_render_mat_color_cube(app, &g_cube, model, view, proj, camera_view_pos,
                              &g_light);
  }
}

internal void app_update(app_s* app, float dt) {
  app_scene_mat_view_render(app, dt);
}

// internal void app_render_mat_color_cube(app_s* app, mesh_renderer_s* cube,
//                                         glm::mat4 model, glm::mat4 view,
//                                         glm::mat4 proj,
//                                         glm::vec3 camera_view_pos,
//                                         light_s* light) {
//   shader_use(cube->shader);
//   mat_color_apply(app->mat_color, cube->shader);
//   mr_set_light(cube, light);
//   mr_set_projection(cube, model, view, proj, camera_view_pos);
//   mr_render(cube);
// }

internal void app_render_mat_color_cube(app_s* app, mesh_renderer_s* cube,
                                        glm::mat4 model, glm::mat4 view,
                                        glm::mat4 proj,
                                        glm::vec3 camera_view_pos,
                                        light_s* light) {
  shader_use(cube->shader);
  mat_tex_apply(app->mat_tex, cube->shader);
  mr_set_light(cube, light);
  mr_set_projection(cube, model, view, proj, camera_view_pos);
  mr_render(cube);
}