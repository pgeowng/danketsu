#include "light.h"

void app_scene_mat_view_render(App *app, float dt) {
  Camera *camera = &app->camera;
  flycamera_update(camera, dt);

  float time = SDL_GetTicks() / 1000.0f;

  glm::mat4 model = glm::mat4(1.0f);

  glm::vec3 light_pos(0.0f);
  update_move_zigzag(&light_pos);
  app->p_light[0].position = light_pos;

  update_color_rainbow(&app->p_light[0]);

  app->p_light[1].position =
      glm::vec3(glm::vec4(1.2f + cos(time), 1.0f, 2.0f + sin(time), 1.0f));

  app->p_light[2].position = glm::vec3(glm::vec4(1.0f,
                                                 1.2f, // + cos(time),
                                                 2.0f, // + sin(time) * 1.1f,
                                                 1.0f));

  app->p_light[3].position = glm::vec3(glm::vec4(1.2f, // + cos(time) * .98f,
                                                 2.0f, // + sin(time) * 2.3,
                                                 1.0f, 1.0f));

  // glm::vec3 light_view_pos = glm::vec3(view * glm::vec4(light_pos, 1.0f));
  // g_light.position = light_view_pos;
  light_s g_light = app->p_light[0];

  app_update_dirlight(&app->dir_light, camViewMat(camera));

  shader_use(&app->lighting_shader);
  shader_1i(&app->lighting_shader, "material.diffuse", 0);
  shader_1i(&app->lighting_shader, "material.specular", 1);

  // for (int i = 0; i < 4; i++) {
  //   draw_lamp(app, &app->p_light[i], view, proj);
  // }

  draw_material_preview(app, &app->camera);

  if (app->enable_maze) {
    draw_cube(app, camera);
    draw_maze(app, camera);
    draw_ramp1(app, camera);
    draw_ramp2(app, camera);
  }

  {
    glm::mat4 model = glm::mat4(1.0f);
    app->mat_color = g_mat_sh_0;
    app_render_mat_color_cube(app, &app->texture_cube_mesh,
                              &app->lighting_shader, model, camera,
                              &app->p_light[0]);
  }

  int text_y = 20;
  text_draw(&app->text_renderer, 10, 20, "Hello, world!");
  text_y += 32;
  char buf[50];
  for (int i = 0; i < 4; i++) {
    sprintf(buf, "[%.2f; %.2f; %.2f]", app->p_light[i].position.x,
            app->p_light[i].position.y, app->p_light[i].position.z);
    text_draw(&app->text_renderer, 10, text_y, buf);
    text_y += 32;
  }
}

internal void app_update(App *app, float dt) {
  app_scene_mat_view_render(app, dt);
}

internal void draw_cube(App *app, Camera *camera) {
  glm::mat4 model = glm::mat4(1.0f);
  app->mat_color = g_mat_sh_0;
  app_render_mat_color_cube(app, &app->cube_mesh, &app->lighting_shader, model,
                            camera, &app->p_light[0]);
}

internal void draw_maze(App *app, Camera *camera) {
  float cell_width = 2.0f;
  float cell_height = 4.3f;
  float cell_depth = 2.3f;

  float cell_fluctuation = 0.5f;
  static int save_rnd = rnd;
  rnd = save_rnd;

  for (int l = 0; l < 3; l++) {

    for (int i = 0; i < g_maze_size; i++) {
      for (int j = 0; j < g_maze_size; j++) {
        if (g_maze[l * g_maze_size * g_maze_size + i * g_maze_size + j] == 1) {

          glm::mat4 model = glm::mat4(1.0f);
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
                            float(rnd = (rnd * rnd_step) % rnd_mod) / rnd_mod));

          ;
          app->mat_color = g_mat_sh_1;
          app_render_mat_color_cube(app, &app->cube_mesh, &app->lighting_shader,
                                    model, camera, &app->p_light[0]);
        }
      }
    }
  }
}

internal void draw_ramp1(App *app, Camera *camera) {
  glm::mat4 model = glm::mat4(1.0f);
  float time = SDL_GetTicks() / 1000.0f;
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
  model = glm::rotate(model, glm::radians(SDL_GetTicks() / 1000.0f * 50.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(
                                // cos(time / 2.0f) +
                                2.0f,
                                // sin(time / 3.0f) +
                                1.3f, 1.0f));

  app->mat_color = g_mat_sh_2;
  app_render_mat_color_cube(app, &app->ramp_mesh, &app->lighting_shader, model,
                            camera, &app->p_light[0]);
}

internal void draw_ramp2(App *app, Camera *camera) {
  float time = SDL_GetTicks() / 1000.0f;

  glm::mat4 model = glm::mat4(1.0f);
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
  app_render_mat_color_cube(app, &app->ramp_mesh, &app->lighting_shader, model,
                            camera, &app->p_light[0]);
}
internal void update_move_zigzag(glm::vec3 *pos) {
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

  *pos = offset;
}

internal void update_color_rainbow(light_s *l) {
  // float time = SDL_GetTicks() / 1000.0f;
  float time = 0;

  glm::vec3 light_color((sin(time * 2.0f) + 1.0f) / 2.0f,
                        (sin(time * 0.7f) + 1.0f) / 2.0f,
                        (sin(time * 1.3f) + 1.0f) / 2.0f);

  l->diffuse = light_color * glm::vec3(0.5f);
  l->ambient = light_color * glm::vec3(0.2f);
  // g_light.specular =
  //     glm::vec3((light_color.r + light_color.g + light_color.b) / 3.0f);
  // g_light.specular = glm::vec3(glm::compMax(light_color));
  l->specular = light_color;
}

internal void app_update_dirlight(light_s *l, glm::mat4 view) {
  // must be in view space
  static glm::vec4 dir_light_direction(0.3f, 0.2f, -0.2f, 0.0f);
  l->direction = glm::vec3(view * dir_light_direction);
}

internal void app_render_mat_color_cube(App *app, mesh_s *mesh, shader_s *sh,
                                        glm::mat4 model, Camera *camera,
                                        light_s *light) {

  shader_use(sh);
  // if (app->enable_mat_color) {
  // mat_color_apply(app->mat_color, sh);
  // } else {
  mat_tex_apply(app->mat_tex, sh);
  // }

  light->direction = camViewDirection(camera);

  shader_set_dirlight(sh, &app->dir_light);

  for (int i = 0; i < 4; i++) {
    app->p_light[i].position = vec3(vec4(app->p_light[i].position, 1.0f));
    shader_set_pointlight(sh, camViewMat(camera), &app->p_light[i], i);
  }

  shader_set_spotlight(sh, &app->sp_light);

  shader_set_light(sh, light);
  shader_set_transform_and_viewpos(sh, model, camViewMat(camera),
                                   camProjMat(camera), camViewPosition(camera));
  mesh_draw(mesh, sh);
}

internal void draw_lamp(App *app, light_s *light, mat4 view, mat4 proj) {
  mat4 model(1.0f);
  model = translate(model, light->position);
  model = scale(model, glm::vec3(0.2f));

  shader_use(&app->lamp_shader);
  shader_set_lightsrc(&app->lamp_shader, light->specular);
  shader_set_transform(&app->lamp_shader, model, view, proj);

  mesh_draw(&app->cube_mesh, &app->lamp_shader);
}

internal void draw_material_preview(App *app, Camera *camera) {
  int columns = 6;

  for (int i = 0; i < COUNT_OF(g_mat_color_materials); i++) {
    int row = i / columns;
    int col = i % columns;
    glm::vec3 pos = glm::vec3(col * 1.0f, row * 1.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::rotate(model, glm::radians(20.0f * i),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

    app->mat_color = g_mat_color_materials[i];

    vec3 rayIntersection = {0, 0, 0};

    bool colliding =
        intersectRayMesh(camera->position - pos, camera->front,
                         &app->texture_cube_mesh, &rayIntersection);

    if (colliding) {
      glm::mat4 sphere_view = glm::mat4(1.0f);
      sphere_view = glm::translate(sphere_view, rayIntersection);
      sphere_view = glm::scale(sphere_view, glm::vec3(0.2));
      printf("colliding (%.2f %.2f %.2f)\n", rayIntersection.x,
             rayIntersection.y, rayIntersection.z);
      app_render_mat_color_cube(app, &app->debug_sphere, &app->lighting_shader,
                                sphere_view, camera, &app->p_light[0]);
    }
    // } else {
    app_render_mat_color_cube(app, &app->texture_cube_mesh,
                              &app->lighting_shader, model, camera,
                              &app->p_light[0]);
    // }
  }
}