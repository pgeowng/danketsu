#include "light.h"

int gScreenWidth = 1000;
int gScreenHeight = 1000;

bool app_init(Scene *app) {
  bool ok = false;
  flycamera_init(&app->camera, false, 60.0f, gScreenWidth / gScreenHeight);
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

  // TODO: need memory allocation
  static mesh_s cubeMesh = {};
  MeshZero(&cubeMesh);
  MeshSetCube(&cubeMesh);
  MeshInitialize(&cubeMesh);

  // MeshZero(&app->cube_mesh);
  // mesh_read_obj(&app->cube_mesh, "assets/icosphere.obj");
  // mesh_add_texture(&app->cube_mesh, "assets/buddy_tex1.png",
  // "material.diffuse"); MeshInitialize(&app->cube_mesh);

  // MeshZero(&app->ramp_mesh);
  // mesh_init_ramp(&app->ramp_mesh);
  // MeshInitialize(&app->ramp_mesh);

  MeshZero(&app->texture_cube_mesh);
  mesh_read_obj(&app->texture_cube_mesh, "assets/checker_cube.obj");
  mesh_add_texture(&app->texture_cube_mesh, "assets/checker.png",
                   "material.diffuse");
  MeshInitialize(&app->texture_cube_mesh);

  MeshZero(&app->debug_sphere);
  mesh_read_obj(&app->debug_sphere, "assets/sphere.obj");
  MeshInitialize(&app->debug_sphere);

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
  glm::vec3 orange(1.0f, 85.0f / 255.0f, 0.0f);
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

  for (int i = 0; i < 4; i++) {
    app->go[i].instance = LampInstance;
    app->go[i].transform = mat4(1.0f);
    app->go[i].mesh = &cubeMesh;
    app->go[i].shader = &app->lamp_shader;
    app->go[i].light = &app->p_light[i];
  }

  return ok;
}
