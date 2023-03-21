#include "light.h"

void app_scene_mat_view_render(Scene *app, float dt) {
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

  draw_material_preview(app, &app->camera);

  // if (app->enable_maze) {
  // draw_maze(app, camera);
  // draw_ramp1(app, camera);
  // draw_ramp2(app, camera);
  // }

  for (int i = 0; i < GOSize; i++) {
    GameObject *obj = &app->go[i];
    switch (obj->instance) {
    case LampInstance: {
      sceneLampDraw(app, obj);
      break;
    }
    case BoxInstance: {
      sceneRenderMatColor(app, obj);
      break;
    }
    case MazeInstance: {
      // sceneRenderMatColor(app, obj);
      // draw_ramp1(app, camera);
      // draw_ramp2(app, camera);
      break;
    }

    case Ramp1Instance: {
      float time = SDL_GetTicks() / 1000.0f;

      glm::mat4 tr = glm::mat4(1.0f);
      tr = glm::translate(tr, glm::vec3(2.0f, 0.0f, 2.0f));
      tr = glm::rotate(tr, glm::radians(time * 50.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
      tr = glm::scale(tr, glm::vec3(
                              // cos(time / 2.0f) +
                              2.0f,
                              // sin(time / 3.0f) +
                              1.3f, 1.0f));

      obj->transform = tr;

      sceneRenderMatColor(app, obj);
      break;
    }

    default: {
      break;
    }
    }
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

internal void app_update(Scene *app, float dt) {
  app_scene_mat_view_render(app, dt);
}

internal void draw_ramp2(Scene *app, Camera *camera) {
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
                            camera);
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

internal void app_render_mat_color_cube(Scene *app, mesh_s *mesh, shader_s *sh,
                                        glm::mat4 model, Camera *camera) {

  shader_use(sh);
  // if (app->enable_mat_color) {
  // mat_color_apply(app->mat_color, sh);
  // } else {
  mat_tex_apply(app->mat_tex, sh);
  // }
  light_s *light = sceneDefaultLight(app);
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
  MeshDraw(mesh, sh);
}

internal void sceneRenderMatColor(Scene *scn, GameObject *obj) {
  Camera *cam = &scn->camera;
  shader_s *sh = obj->shader;
  // light_s *light = obj->light;
  light_s *light = sceneDefaultLight(scn);

  shader_use(sh);
  if (obj->mat_color != NULL) {
    mat_color_apply(*obj->mat_color, sh);
  } else {
    // mat_tex_apply(scn->mat_tex, object->shader);
  }

  // TODO: actually bad thing
  // obj->light->direction = camViewDirection(cam);

  shader_set_dirlight(sh, &scn->dir_light);

  for (int i = 0; i < 4; i++) {
    scn->p_light[i].position = vec3(vec4(scn->p_light[i].position, 1.0f));
    shader_set_pointlight(sh, camViewMat(cam), &scn->p_light[i], i);
  }

  shader_set_spotlight(sh, &scn->sp_light);

  shader_set_light(sh, light);
  shader_set_transform_and_viewpos(sh, obj->transform, camViewMat(cam),
                                   camProjMat(cam), camViewPosition(cam));
  MeshDraw(obj->mesh, sh);
}

internal void sceneLampDraw(Scene *scene, GameObject *lamp) {
  light_s *light = lamp->light;
  shader_s *shader = lamp->shader;

  lamp->transform = mat4(1.0f);
  lamp->transform = translate(lamp->transform, light->position);
  lamp->transform = scale(lamp->transform, glm::vec3(.2f));

  shader_use(shader);
  shader_set_lightsrc(shader, light->specular);
  shader_set_transform(shader, lamp->transform, camViewMat(&scene->camera),
                       camProjMat(&scene->camera));

  MeshDraw(lamp->mesh, shader);
}

internal void draw_material_preview(Scene *app, Camera *camera) {
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
                                sphere_view, camera);
    }
    // } else {
    app_render_mat_color_cube(app, &app->texture_cube_mesh,
                              &app->lighting_shader, model, camera);
    // }
  }
}

internal light_s *sceneDefaultLight(Scene *scn) {
  for (int i = 0; i < GOSize; i++) {
    GameObject *obj = &scn->go[i];
    if (obj->light != NULL) {
      return obj->light;
    }
  }

  return &sceneMissingLight;
}