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


  // mesh_init(&app->cube_mesh);
  // mesh_init_cube_tex(&app->cube_mesh);
  // mesh_setup(&app->cube_mesh);

  mesh_init(&app->cube_mesh);
  mesh_read_obj(&app->cube_mesh, "assets/icosphere.obj");
  mesh_add_texture(&app->cube_mesh, "assets/buddy_tex1.png", "material.diffuse");
  mesh_setup(&app->cube_mesh);

  mesh_init(&app->ramp_mesh);
  mesh_init_ramp(&app->ramp_mesh);
  mesh_setup(&app->ramp_mesh);


  // g_cube.shader = &app->lighting_shader;
  // g_cube.vao = app->vao;
  // g_cube.vbo = app->vbo;
  // g_cube.num_triangles = 72;

  // g_ramp.shader = &app->lighting_shader;
  // g_ramp.vao = app->ramp_vao;
  // g_ramp.vbo = app->ramp_vbo;
  // g_ramp.num_triangles = 24;

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
    .tex_emission = 0,
    .tex_diffuse_unit = GL_TEXTURE1,
    .tex_specular_unit = GL_TEXTURE2,
    .tex_emission_unit = GL_TEXTURE3,
    .tex_diffuse_unit_idx = 1,
    .tex_specular_unit_idx = 2,
    .tex_emission_unit_idx = 3,
    .shininess = 32.0f,
  };

  ok = tex_load(&app->mat_tex.tex_diffuse, "assets/container2.png");
  if (!ok) {
    return ok;
  }

  glActiveTexture(app->mat_tex.tex_diffuse_unit);
  glBindTexture(GL_TEXTURE_2D, app->mat_tex.tex_diffuse);

  ok = tex_load(&app->mat_tex.tex_specular, "assets/container2_specular2.png");
  if (!ok) {
    return ok;
  }

  ok = tex_load(&app->mat_tex.tex_emission, "assets/matrix.jpg");
  if (!ok) {
    return ok;
  }

  glActiveTexture(app->mat_tex.tex_specular_unit);
  glBindTexture(GL_TEXTURE_2D, app->mat_tex.tex_specular);

  printf("mat tex: %f %d %d %d %d", app->mat_tex.shininess,
         app->mat_tex.tex_diffuse, app->mat_tex.tex_diffuse_unit,
         app->mat_tex.tex_specular, app->mat_tex.tex_specular_unit);

  // dirlight
  app->dir_light.ambient = glm::vec3(0.1f);
  app->dir_light.diffuse = glm::vec3(0.5f);
  app->dir_light.specular = glm::vec3(1.0f);

  // pointlight
  for (int i = 0; i < 4; i++) {
    app->p_light[i].constant = 1.0f;
    app->p_light[i].linear = 0.09f;
    app->p_light[i].quadratic = 0.032f;
  }

  glm::vec3 purple(255.0f / 255.0f, 115.0f / 255.0f, 253.0f / 255.0f);
  app->p_light[1].specular = purple;
  app->p_light[1].diffuse = purple * 0.5f;
  app->p_light[1].ambient = purple * 0.1f;

  glm::vec3 yellow(255.0f / 255.0f, 215.0f / 255.0f, 0.0f);
  app->p_light[2].specular = yellow;
  app->p_light[2].diffuse = yellow * 0.5f;
  app->p_light[2].ambient = yellow * 0.1f;

  glm::vec3 blue(23.0f / 255.0f, 159.0f / 255.0f, 255.0f / 255.0f);
  app->p_light[3].specular = blue;
  app->p_light[3].diffuse = blue * 0.5f;
  app->p_light[3].ambient = blue * 0.1f;

  // spotlight
  glm::vec3 orange(1.0f, 85.0f/255.0f, 0.0f);
  app->sp_light.specular = orange;
  app->sp_light.diffuse = orange * 0.5f;
  app->sp_light.ambient = orange * 0.1f;
  app->sp_light.position = glm::vec3(0.0f);
  app->sp_light.direction = glm::vec3(0.0f, 0.0f, -1.0f);
  app->sp_light.cutOff = glm::cos(glm::radians(12.5f));
  app->sp_light.outerCutOff = glm::cos(glm::radians(15.5f));

  ok = text_init(&app->text_renderer);
  if (!ok) {
    printf("light: failed to init text renderer\n");
    return ok;
  }

  return ok;
}

void app_scene_mat_view_render(app_s* app, float dt) {
  flycamera_update(&app->camera, dt);

  float time = SDL_GetTicks() / 1000.0f;

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = flycamera_get_view_matrix(&app->camera);
  glm::mat4 proj = flycamera_get_projection_matrix(&app->camera);

  glm::vec3 light_pos(0.0f);
  update_move_zigzag(&light_pos);
  app->p_light[0].position = light_pos;

  update_color_rainbow(&app->p_light[0]);

  app->p_light[1].position = glm::vec3(
    glm::vec4(1.2f + cos(time), 1.0f, 2.0f + sin(time), 1.0f));


  app->p_light[2].position = glm::vec3(
      glm::vec4(1.0f, 1.2f + cos(time), 2.0f + sin(time) * 1.1f, 1.0f));

  app->p_light[3].position = glm::vec3(
      glm::vec4(1.2f + cos(time) * .98f, 2.0f + sin(time) * 2.3, 1.0f, 1.0f));

  // glm::vec3 light_view_pos = glm::vec3(view * glm::vec4(light_pos, 1.0f));
  // g_light.position = light_view_pos;
  light_s g_light = app->p_light[0];

  app_update_dirlight(&app->dir_light, view);

  shader_use(&app->lighting_shader);
  shader_1i(&app->lighting_shader, "material.diffuse", 0);
  shader_1i(&app->lighting_shader, "material.specular", 1);

  glm::vec3 camera_view_pos =
      glm::vec3(view * glm::vec4(app->camera.position, 1.0f));

  for (int i = 0; i < 4; i++) {
    draw_lamp(app, &app->p_light[i], view, proj);
  }

  draw_material_preview(app, view, proj, camera_view_pos);

  if (app->enable_maze) {
    draw_cube(app, view, proj, camera_view_pos);
    draw_maze(app, view, proj, camera_view_pos);
    draw_ramp1(app, view, proj, camera_view_pos);
    draw_ramp2(app, view, proj, camera_view_pos);
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

internal void app_update(app_s* app, float dt) {
  app_scene_mat_view_render(app, dt);
}

internal void draw_cube(app_s* app, glm::mat4 view, glm::mat4 proj,
                        glm::vec3 camera_view_pos) {
  glm::mat4 model = glm::mat4(1.0f);
  app->mat_color = g_mat_sh_0;
  app_render_mat_color_cube(app, &app->cube_mesh, &app->lighting_shader, model, view, proj, camera_view_pos,
                            &app->p_light[0]);
}

internal void draw_maze(app_s* app, glm::mat4 view, glm::mat4 proj,
                        glm::vec3 camera_view_pos) {
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
          app_render_mat_color_cube(app, &app->cube_mesh, &app->lighting_shader, model, view, proj,
                                    camera_view_pos, &app->p_light[0]);
        }
      }
    }
  }
}

internal void draw_ramp1(app_s* app, glm::mat4 view, glm::mat4 proj,
                         glm::vec3 camera_view_pos) {
  glm::mat4 model = glm::mat4(1.0f);
  float time = SDL_GetTicks() / 1000.0f;
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
  model = glm::rotate(model, glm::radians(SDL_GetTicks() / 1000.0f * 50.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(
      model, glm::vec3(cos(time / 2.0f) + 2.0f, sin(time / 3.0f) + 1.3f, 1.0f));

  app->mat_color = g_mat_sh_2;
  app_render_mat_color_cube(app, &app->ramp_mesh, &app->lighting_shader, model, view, proj, camera_view_pos,
                            &app->p_light[0]);
}

internal void draw_ramp2(app_s* app, glm::mat4 view, glm::mat4 proj,
                         glm::vec3 camera_view_pos) {
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
  app_render_mat_color_cube(app, &app->ramp_mesh,&app->lighting_shader, model, view, proj, camera_view_pos,
                            &app->p_light[0]);
}
internal void update_move_zigzag(glm::vec3* pos) {
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

internal void update_color_rainbow(light_s* l) {
  float time = SDL_GetTicks() / 1000.0f;

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

internal void app_update_dirlight(light_s* l, glm::mat4 view) {
  // must be in view space
  static glm::vec4 dir_light_direction(0.3f, 0.2f, -0.2f, 0.0f);
  l->direction = glm::vec3(view * dir_light_direction);
}

internal void app_render_mat_color_cube(app_s* app, mesh_s* mesh, shader_s* sh,
                                        glm::mat4 model, glm::mat4 view,
                                        glm::mat4 proj,
                                        glm::vec3 camera_view_pos,
                                        light_s* light) {

  shader_use(sh);
  // if (app->enable_mat_color) {
    // mat_color_apply(app->mat_color, sh);
  // } else {
    mat_tex_apply(app->mat_tex, sh);
  // }

  light->direction = glm::vec3(view * glm::vec4(app->camera.front, 0.0f));

  shader_set_dirlight(sh, &app->dir_light);

  for (int i = 0; i < 4; i++) {
    app->p_light[i].position = vec3(vec4(app->p_light[i].position, 1.0f));
    shader_set_pointlight(sh, view, &app->p_light[i], i);
  }

  shader_set_spotlight(sh, &app->sp_light);

  shader_set_light(sh, light);
  shader_set_transform_and_viewpos(sh, model, view, proj, camera_view_pos);
  mesh_draw(mesh, sh);
}

internal void draw_lamp(app_s* app, light_s* light, mat4 view, mat4 proj) {
  mat4 model(1.0f);
  model = translate(model, light->position);
  model = scale(model, glm::vec3(0.2f));

  shader_use(&app->lamp_shader);
  shader_set_lightsrc(&app->lamp_shader, light->specular);
  shader_set_transform(&app->lamp_shader, model, view, proj);

  mesh_draw(&app->cube_mesh, &app->lamp_shader);
}

internal void draw_material_preview(app_s* app, mat4 view, mat4 proj,
                                    vec3 camera_view_pos) {
  int columns = 6;

  for (int i = 0; i < COUNT_OF(g_mat_color_materials); i++) {
    int row = i / columns;
    int col = i % columns;
    glm::vec3 pos = glm::vec3(col * 1.0f, row * 1.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

    app->mat_color = g_mat_color_materials[i];
    app_render_mat_color_cube(app, &app->cube_mesh, &app->lighting_shader, model, view, proj, camera_view_pos,
                              &app->p_light[0]);
  }
}

internal void app_input(app_s* app, SDL_Event e) {
  static int input_move_forward = 0;
  static int input_move_right = 0;
  static int input_move_left = 0;
  static int input_move_back = 0;
  static SDL_bool g_relative_mouse_mode = SDL_FALSE;
  switch (e.type) {
  case SDL_WINDOWEVENT: {
    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
      int g_screenWidth = e.window.data1;
      int g_screenHeight = e.window.data2;
      glViewport(0, 0, g_screenWidth, g_screenHeight);
      // is it good to do this here?
      app->camera.aspect = (float)g_screenWidth / (float)g_screenHeight;
    }
    break;
  }
  case SDL_KEYDOWN:
  case SDL_KEYUP: {
    float forward = 0;
    float right = 0;
    int pressed = e.key.state == SDL_PRESSED;
    switch (e.key.keysym.sym) {
    case SDLK_w: {
      input_move_forward = pressed;
      break;
    }
    case SDLK_a: {
      input_move_left = pressed;
      break;
    };
    case SDLK_s: {
      input_move_back = pressed;
      break;
    }
    case SDLK_d: {
      input_move_right = pressed;
      break;
    }
    case SDLK_e: {
      if (!pressed) {
        app->enable_maze = !app->enable_maze;
      }
      break;
    }
    case SDLK_q: {
      if (!pressed) {
        app->enable_mat_color = !app->enable_mat_color;
      }
      break;
    }
    case SDLK_ESCAPE: {
      if (!pressed) {
        g_relative_mouse_mode =
            (g_relative_mouse_mode == SDL_TRUE) ? SDL_FALSE : SDL_TRUE;
        int ok = SDL_SetRelativeMouseMode(g_relative_mouse_mode);
        if (ok) {
          printf("sdl set relative mouse mode failed: %s\n", SDL_GetError());
        }
      }
      break;
    }
    case SDLK_LSHIFT: {
      app->camera.speed = pressed ? 10.0f : 5.0f;
      break;
    }
    }

    flycamera_process_movement(&app->camera,
                               input_move_forward - input_move_back,
                               input_move_right - input_move_left);
    break;
  }

  case SDL_MOUSEMOTION: {
    flycamera_process_mouse_movement(&app->camera, e.motion.xrel,
                                     e.motion.yrel);
    break;
  }

  case SDL_MOUSEWHEEL: {
    flycamera_process_mouse_scroll(&app->camera, e.wheel.y);
    break;
  }
  }
}