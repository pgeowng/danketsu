#ifndef MESH_H
#define MESH_H


#include "alloc.h"
#include "shader.h"

#include "unity.h"

struct vertex_s {
  vec3 pos;
  vec3 normal;
  vec2 texcoord;
};

struct texture_s {
  GLuint id;
  const char* type;
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

void mesh_zero(mesh_s *m);
bool mesh_read_obj(mesh_s *m, const char * filename);
bool mesh_setup(mesh_s *m);
bool mesh_clean(mesh_s *m);
void mesh_draw(mesh_s *m, shader_s *sh);


// #define MAX_VERTICES 20000
// #define MAX_INDICES 20000
// #define MAX_TEXTURES 8


// struct texture_s {
//     unsigned int id;
//     const char *type;
// };

// struct mesh_s {
//     vertex_s* vertices;
//     int num_vertices;

//     unsigned int * indices;
//     int num_indices;

//     texture_s * textures;
//     int num_textures;
// };

// bool mesh_zero(mesh_s *mesh_s, );

// // struct mesh_renderer_s {
// //     mesh_s mesh;
// //     unsigned int vao;
// //     unsigned int vbo;
// //     unsigned int ebo;
// //     unsigned int num_vertices;
// //     unsigned int num_indices;
// //     unsigned int num_textures;
// // };



#endif