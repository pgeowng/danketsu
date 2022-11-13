#ifndef HELLO_H
#define HELLO_H

#include "unity.h"

#include "flycamera.h"
#include "shader.h"
#include "texture.h"

struct app_s {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  flycamera_s camera = {};
  shader_s shader = {};

  GLuint texWall;
  GLuint texAwesome;
};

#endif