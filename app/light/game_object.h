#include "unity.h"

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "light_shader.h"
#include "mat_color.cpp"
#include "mesh.h"
#include "shader.h"

struct GameObject {

  Uint16 instance;

  // Model matrix translates object to the world coordinates.
  mat4 transform;

  // Mesh contains GameObject render mesh if it has one.
  mesh_s *mesh;

  // Shader tells how GameObject should be displayed.
  shader_s *shader;

  // Light tells that GameObject emits light.
  light_s *light;

  // MaterialColor stores parameters for color shader rendering.
  mat_color_s *mat_color;
};

internal void GameObjectConstruct(GameObject *obj, uint16_t instance,
                                  mat4 transform, mesh_s *mesh,
                                  shader_s *shader, light_s *light,
                                  mat_color_s *mat_color);
#endif