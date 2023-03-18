#include "hello.h"

bool app_init(app_s *app) {
  bool ok = false;
  flycamera_init(&app->camera);
  ok = shader_init(&app->shader, "./app/hello/hello.vert",
                   "./app/hello/hello.frag");
  if (!ok) {
    printf("shader new failed");
    return ok;
  }

  mesh_zero(&app->cube_mesh);
  mesh_init_cube(&app->cube_mesh);
  mesh_setup(&app->cube_mesh);

  shader_use(&app->shader);

  ok = tex_load(&app->texWall, "assets/wall.jpg");
  if (!ok) {
    return false;
  }
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, app->texWall);

  ok = tex_load(&app->texAwesome, "assets/awesomeface.png");
  if (!ok) {
    return false;
  }
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, app->texAwesome);

  return ok;
}

void app_update(app_s *app, float delta) {
  static float timeValue = 0.0f;
  timeValue += delta;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(20.0f * timeValue),
                      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  flycamera_update(&app->camera, delta);

  shader_use(&app->shader);
  shader_1i(&app->shader, "tex1", 0);
  shader_1i(&app->shader, "tex2", 1);
  shader_mat4fv(&app->shader, "model", glm::value_ptr(model));
  shader_mat4fv(&app->shader, "view", glm::value_ptr(fcamView(&app->camera)));
  // shader_SetMatrix4fv(&app->shader, "view",
  // glm::value_ptr(flycamera_get_weird_view_matrix(&app->camera)));
  shader_mat4fv(&app->shader, "projection",
                glm::value_ptr(fcamProjection(&app->camera)));

  mesh_draw(&app->cube_mesh, &app->shader);

  glm::vec3 positions[] = {
      glm::vec3(0.2f, -0.2f, 1.0f),
      glm::vec3(-0.4f, 0.4f, 0.5f),
      glm::vec3(0.0f, 0.0f, 1.5f),
      glm::vec3(0.89f, 0.5f, 2.0f),
  };

  for (int i = 0; i < 3; i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, positions[i]);
    model = glm::rotate(model, glm::radians(timeValue * 25 * (i + 1)),
                        positions[i]);

    shader_mat4fv(&app->shader, "model", glm::value_ptr(model));

    mesh_draw(&app->cube_mesh, &app->shader);
  }
}