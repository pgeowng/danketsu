#include "light.h"

int gScreenWidth = 1000;
int gScreenHeight = 1000;

bool app_init(Scene *app) {
  // TODO: Dependency Injecition.
  app->arena = MArenaMake(MMallocBaseMemory());

  U64 objectCount = 10;
  GameObject *objs =
      (GameObject *)MArenaPush(&app->arena, sizeof(GameObject) * objectCount);

  GameObject *obj2 = PushArray(&app->arena, GameObject, objectCount);

  bool ok = false;
  flycamera_init(&app->camera, false, 60.0f,
                 (float)(gScreenWidth) / gScreenHeight);
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
  static shader_s colorShader;
  ok = shader_init(&colorShader, "./app/light/light.vert",
                   "./app/light/light_color.frag");
  if (!ok) {
    printf("colorShader initialization failed");
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

  MeshZero(&app->texture_cube_mesh);
  mesh_read_obj(&app->texture_cube_mesh, &app->arena,
                "assets/checker_cube.obj");
  mesh_add_texture(&app->texture_cube_mesh, "assets/checker.png",
                   "material.diffuse");
  MeshInitialize(&app->texture_cube_mesh);

  MeshZero(&app->debug_sphere);
  mesh_read_obj(&app->debug_sphere, &app->arena, "assets/sphere.obj");
  MeshInitialize(&app->debug_sphere);

  // g_cube.shader = &app->lighting_shader;
  // g_cube.vao = app->vao;
  // g_cube.vbo = app->vbo;
  // g_cube.num_triangles = 72;

  // g_ramp.shader = &app->lighting_shader;
  // g_ramp.vao = app->ramp_vao;
  // g_ramp.vbo = app->ramp_vbo;
  // g_ramp.num_triangles = 24;

  {
    MaterialTextureSlot diffuse, specular, emission;

    {
      diffuse = NewMaterialTextureSlot(MaterialTextureSlotKind::Diffuse,
                                       GL_TEXTURE1, 1);
      ok = tex_load(&diffuse.tex, "assets/container2.png");
      if (!ok) {
        return ok;
      }

      // TODO: error prone
      glActiveTexture(diffuse.unit);
      glBindTexture(GL_TEXTURE_2D, diffuse.tex);
    }

    {
      specular = NewMaterialTextureSlot(MaterialTextureSlotKind::Specular,
                                        GL_TEXTURE2, 2);
      ok = tex_load(&specular.tex, "assets/container2_specular2.png");
      if (!ok) {
        return ok;
      }
    }

    {
      emission = NewMaterialTextureSlot(MaterialTextureSlotKind::Emission,
                                        GL_TEXTURE3, 3);
      ok = tex_load(&emission.tex, "assets/matrix.jpg");
      if (!ok) {
        return ok;
      }

      glActiveTexture(emission.unit);
      glBindTexture(GL_TEXTURE_2D, emission.tex);
    }

    app->mat_tex = NewMaterialTexture(diffuse, specular, emission);
  }

  app->dir_light = NewDirLight(vec3(.1f), vec3(.5f), vec3(1.f));

  {
    vec3 white(1.f, 1.f, 1.f);
    vec3 purple(255.0f / 255.0f, 115.0f / 255.0f, 253.0f / 255.0f);
    vec3 yellow(255.0f / 255.0f, 215.0f / 255.0f, 0.0f);
    vec3 blue(23.0f / 255.0f, 159.0f / 255.0f, 255.0f / 255.0f);
    vec3 colors[4] = {
        white,
        purple,
        yellow,
        blue,
    };

    // pointlight
    for (int i = 0; i < 4; i++) {
      vec3 c = colors[i];
      app->p_light[i] = NewPointLight(c * .1f, c * .5f, c, 1.0f, .09f, .032f);
    }
  }

  // spotlight
  {
    glm::vec3 orange(1.0f, 85.0f / 255.0f, 0.0f);
    app->sp_light = NewSpotLight(orange * .1f, orange * .5f, orange,
                                 glm::cos(glm::radians(12.5f)),
                                 glm::cos(glm::radians(15.5f)));
  }

  // TODO: light should know it's position
  // app->sp_light.position = glm::vec3(0.0f);
  // app->sp_light.direction = glm::vec3(0.0f, 0.0f, -1.0f);

  ok = text_init(&app->text_renderer);
  if (!ok) {
    printf("light: failed to init text renderer\n");
    return ok;
  }

  int idx = 0;
  int light_i = 0;
  GameObjectConstruct(&app->go[idx++], Lamp1Instance, mat4(1.0), &cubeMesh,
                      &app->lamp_shader, &app->p_light[light_i++], NULL);
  GameObjectConstruct(&app->go[idx++], Lamp2Instance, mat4(1.0), &cubeMesh,
                      &app->lamp_shader, &app->p_light[light_i++], NULL);
  GameObjectConstruct(&app->go[idx++], Lamp3Instance, mat4(1.0), &cubeMesh,
                      &app->lamp_shader, &app->p_light[light_i++], NULL);
  GameObjectConstruct(&app->go[idx++], Lamp4Instance, mat4(1.0), &cubeMesh,
                      &app->lamp_shader, &app->p_light[light_i++], NULL);

  {
    // TODO: make dynamic light detection
    GameObjectConstruct(&app->go[idx++], BoxInstance, mat4(1.0f),
                        &app->texture_cube_mesh, &colorShader, &app->p_light[0],
                        &g_mat_sh_0);
  }

  {
    idx += sceneMazeStart(&app->go[idx], &cubeMesh, &app->lighting_shader,
                          &app->p_light[1]);
  }

  {
    mesh_s *rampMesh = PushArray(&app->arena, mesh_s, 1);
    MeshZero(rampMesh);
    MeshSetRamp(rampMesh);
    MeshInitialize(rampMesh);

    GameObjectConstruct(&app->go[idx++], Ramp1Instance, mat4(1.0f), rampMesh,
                        &app->lighting_shader, NULL, &g_mat_sh_2);

    GameObjectConstruct(&app->go[idx++], Ramp2Instance, mat4(1.0f), rampMesh,
                        &app->lighting_shader, NULL, &g_mat_sh_2);
  }

  return ok;
}

internal int sceneMazeStart(GameObject *objectArena, mesh_s *mesh,
                            shader_s *lightingShader, light_s *lightSource) {
  const int blockMaskX = 10;
  const int blockMaskZ = 10;
  const int blockMaskY = 3;

  int blockMask[blockMaskX * blockMaskZ * blockMaskY] = {0};
  {
    for (int l = 0; l < blockMaskY; l++) {
      for (int i = 0; i < blockMaskX; i++) {
        for (int j = 0; j < blockMaskZ; j++) {
          int idx = l * blockMaskX * blockMaskZ + i * blockMaskZ + j;
          blockMask[idx] = (rnd > 60 ? 1 : 0);

          rnd = (rnd * rnd_step) % rnd_mod;
        }
      }
    }
  }
  const float cell_width = 2.0f;
  const float cell_height = 4.3f;
  const float cell_depth = 2.3f;

  const float cell_fluctuation = 0.5f;
  static int save_rnd = rnd;
  rnd = save_rnd;

  int objectIdx = 0;

  for (int l = 0; l < blockMaskY; l++) {
    for (int i = 0; i < blockMaskX; i++) {
      for (int j = 0; j < blockMaskZ; j++) {
        if (blockMask[l * blockMaskX * blockMaskZ + i * blockMaskZ + j] == 1) {

          glm::mat4 transform = glm::mat4(1.0f);
          transform = glm::translate(transform, glm::vec3(2.0f, -2.0f, 2.0f));
          transform = glm::translate(
              transform,
              glm::vec3(i * cell_width - blockMaskX / 2.0f, l * cell_height,
                        j * cell_depth - blockMaskZ / 2.0f));

          transform =
              glm::scale(transform, glm::vec3(cell_width - cell_fluctuation,
                                              cell_height - cell_fluctuation,
                                              cell_depth - cell_fluctuation));

          transform = glm::translate(
              transform,
              glm::vec3(cell_fluctuation *
                            float(rnd = (rnd * rnd_step) % rnd_mod) / rnd_mod,
                        cell_fluctuation *
                            float(rnd = (rnd * rnd_step) % rnd_mod) / rnd_mod,
                        cell_fluctuation *
                            float(rnd = (rnd * rnd_step) % rnd_mod) / rnd_mod));

          ;

          GameObjectConstruct(&objectArena[objectIdx++], MazeInstance,
                              transform, mesh, lightingShader, lightSource,
                              &g_mat_sh_0);
        }
      }
    }
  }

  return objectIdx;
}

void AppClean(Scene *scn) {}