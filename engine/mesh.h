#ifndef MESH_H
#define MESH_H

#include "alloc.h"
#include "log.h"
#include "memory/base_memory.h"
#include "shader.h"
#include "texture.h"


#include "unity.h"

struct vertex_s {
  vec3 pos;
  vec3 normal;
  vec2 texcoord;
};

struct texture_s {
  GLuint id;
  const char *type;
  // const char* path;
};

struct mesh_s {

  vertex_s *verts;
  int verts_size;
  int verts_cap;

  uint *indices;
  int indices_size;
  int indices_cap;

  texture_s *textures;
  int textures_size;
  int textures_cap;

  GLuint vao;
  GLuint vbo;
  GLuint ebo;
};

void meshZero(mesh_s *m);
bool mesh_read_obj(mesh_s *m, MArena *arena, const char *filename);
bool meshInit(mesh_s *m);
bool meshClean(mesh_s *m);

#endif