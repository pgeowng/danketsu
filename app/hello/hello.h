#ifndef HELLO_H
#define HELLO_H

#include "unity.h"

#include "example/cube_mesh.h"
#include "flycamera.h"
#include "mesh.cpp"
#include "shader.h"
#include "texture.h"

struct App {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  Camera camera = {};
  shader_s shader = {};

  mesh_s cube_mesh = {};

  GLuint texWall;
  GLuint texAwesome;
};

#endif