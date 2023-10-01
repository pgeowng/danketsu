#ifndef MATCH3_H
#define MATCH3_H

#include "flycamera.h"
#include "light_shader.h"
#include "mat_color.cpp"
#include "mat_tex.cpp"
#include "mesh.cpp"
#include "mesh.h"

#include "example/cube_mesh.h"

struct Scene {
  Camera camera;
  shader_s shader;
  mesh_s cubeMesh;
  MaterialTexture materialTexture;

  light_s dirLight;
};

internal bool app_init(Scene *app);
internal void app_update(Scene *app, float delta);
internal void app_input(Scene *app, SDL_Event e);
internal void AppClean(Scene *app);

#endif