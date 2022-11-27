#ifndef HELLO_H
#define HELLO_H

#include "unity.h"

#include "flycamera.h"
#include "shader.h"
#include "texture.h"
#include "example/cube_mesh.h"
#include "mesh.cpp"

struct app_s {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  flycamera_s camera = {};
  shader_s shader = {};

  mesh_s cube_mesh = {};

  GLuint texWall;
  GLuint texAwesome;
};

#endif