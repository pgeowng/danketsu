#ifndef MESH_H
#define MESH_H

#include "unity.h"


#define MAX_VERTICES 20000
#define MAX_INDICES 20000
#define MAX_TEXTURES 8

struct vertex_s {
    vec3 pos;
    vec3 normal;
    vec2 texcoord;
};

struct texture_s {
    unsigned int id;
    const char *type;
};

struct mesh_s {
    vertex_s* vertices;
    int num_vertices;

    unsigned int * indices;
    int num_indices;

    texture_s * textures;
    int num_textures;
};

bool mesh_init(mesh_s *mesh_s, );

// struct mesh_renderer_s {
//     mesh_s mesh;
//     unsigned int vao;
//     unsigned int vbo;
//     unsigned int ebo;
//     unsigned int num_vertices;
//     unsigned int num_indices;
//     unsigned int num_textures;
// };



#endif