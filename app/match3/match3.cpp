#include "match3.h"

static bool app_init(Scene *app) {
  cameraInit(&app->camera, 45.0f, 16.0f/9.0f, 0.1f, 100.0f, 0);

  bool ok = false;
  ok = shader_init(&app->shader, "./app/match3/shader.vert",
                   "./app/match3/shader.frag");
  if (!ok) {
    return ok;
  }

  MeshZero(&app->cubeMesh);
  MeshSetCube(&app->cubeMesh);
  MeshInitialize(&app->cubeMesh);

  MaterialTextureSlot diffuse, specular, emission;

  diffuse =
      NewMaterialTextureSlot(MaterialTextureSlotKind::Diffuse, GL_TEXTURE1, 1);
  ok = tex_load(&diffuse.tex, "assets/checker.png");
  if (!ok) {
    return ok;
  }

  glActiveTexture(diffuse.unit);
  glBindTexture(GL_TEXTURE_2D, diffuse.tex);

  specular =
      NewMaterialTextureSlot(MaterialTextureSlotKind::Diffuse, GL_TEXTURE2, 2);
  ok = tex_load(&specular.tex, "assets/checker.png");
  if (!ok) {
    return ok;
  }

  glActiveTexture(specular.unit);
  glBindTexture(GL_TEXTURE_2D, specular.tex);

  emission =
      NewMaterialTextureSlot(MaterialTextureSlotKind::Diffuse, GL_TEXTURE2, 2);
  ok = tex_load(&emission.tex, "assets/checker.png");
  if (!ok) {
    return ok;
  }

  glActiveTexture(emission.unit);
  glBindTexture(GL_TEXTURE_2D, emission.tex);

  app->materialTexture = NewMaterialTexture(diffuse, specular, emission);

  app->dirLight = NewDirLight(vec3(.1f), vec3(.5f), vec3(1.f));

  return true;
}

static void app_input(Scene *app, SDL_Event e) {
  switch (e.type) {
  case SDL_MOUSEMOTION: {
    float xoffset = (float)(e.motion.xrel) / 500;
    float yoffset = (float)(e.motion.yrel) / 500;
    app->camera.position.x += xoffset;
    app->camera.position.y -= yoffset;
    break;
  }
  case SDL_MOUSEWHEEL: {
    flycamera_process_mouse_scroll(&app->camera, (float)(e.wheel.y));
    break;
  }
  }
}

static void app_update(Scene *app, float delta) {
  Camera *camera = &app->camera;

  static float timeValue = 0.0f;
  timeValue += delta;

  glm::mat4 model = glm::mat4(1.0f);
  // model = glm::rotate(model, glm::radians(20.0f * timeValue),
  //                     glm::vec3(0.0f, 1.0f, 0.0f));

  {
    shader_s *shader = &app->shader;
    shader_use(shader);
    // ShaderApplyMaterialTexture(shader, app->materialTexture);
    // shader_1i(shader, "tex1", 0);
    // shader_1i(shader, "tex2", 1);
    // shader_mat4fv(shader, "model", glm::value_ptr(model));
    // shader_mat4fv(shader, "view", glm::value_ptr(camViewMat(camera)));
    // shader_mat4fv(shader, "projection",
    // glm::value_ptr(camProjMat(camera)));

    app->dirLight.position = camera->position;
    // app->dirLight.position.x = glm::sin(timeValue) * 2.0f;
    // app->dirLight.position.y = glm::cos(timeValue) * 2.0f;
    // app->dirLight.position.y = 0.0f;
    // app->dirLight.position.z = glm::sin(timeValue + 1000) * 2.0f;
    // app->dirLight.position.z = 0.0f;
    app->dirLight.position.z =
        app->dirLight.position.z + glm::sin(timeValue + 1000) * 2.0f;

    {
      mesh_s *cubeMesh = &app->cubeMesh;
      shader_set_transform_and_viewpos(
          shader,
          glm::translate(glm::scale(glm::mat4(1.0f), vec3(0.25f)),
                         app->dirLight.position),
          camViewMat(camera), camProjMat(camera), camViewPosition(camera));
      MeshDraw(cubeMesh, shader);
    }

    (&app->dirLight)->direction = camViewDirection(camera);
    // shader_set_dirlight(shader, &app->dirLight);

    ShaderApplyMatLight(shader, g_ruby, app->dirLight);

    mat4 cubesPosition[3] = {
        // glm::translate(glm::mat4(1.0f), vec3(2.0f, 0.0f, 0.0f)),
        // glm::translate(glm::mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)),
        glm::translate(glm::mat4(1.0f), vec3(0.0f, 1.0f, 0.0f))};

    for (int i = 0; i < nuArraySize(cubesPosition); i++) {
      mesh_s *cubeMesh = &app->cubeMesh;
      shader_set_transform_and_viewpos(shader, cubesPosition[i],
                                       camViewMat(camera), camProjMat(camera),
                                       camViewPosition(camera));
      MeshDraw(cubeMesh, shader);
    }
  }
}

static void AppClean(Scene *app) {}