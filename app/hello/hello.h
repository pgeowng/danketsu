#ifndef HELLO_H
#define HELLO_H

#include "unity.h"

#include "example/cube_mesh.h"
#include "flycamera.h"
#include "mesh.cpp"
#include "shader.h"
#include "texture.h"

#include "renderer3d.h"

struct Scene {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  Renderer3D r3;


  mesh_s cube_mesh = {};

  GLuint texWall;
  GLuint texAwesome;
};

static void appDraw(Scene *app);
static void appUpd(Scene *app, f32 delta);

#endif