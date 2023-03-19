#ifndef EXAMPLE_CUBE_MESH_H
#define EXAMPLE_CUBE_MESH_H

#include "alloc.h"
#include "mesh.h"

void MeshSetCube(mesh_s *m) {
  float vertices[] = {// front
                      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                      -0.5f, 0.5f, -0.5f, // Front Left Top
                      0.0f, 1.0f,         // Texture

                      0.5f, 0.5f, -0.5f, // Front Right Top
                      1.0f, 1.0f,        // Texture

                      0.5f, -0.5f, -0.5f, // Front Right Bottom
                      1.0f, 0.0f,         // Texture

                      // right
                      0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                      0.5f, 0.5f, -0.5f, 1.0f, 0.0f,

                      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

                      0.5f, -0.5f, 0.5f, 0.0f, 1.0f,

                      // back
                      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

                      0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

                      -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

                      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,

                      // left
                      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

                      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

                      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

                      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

                      // top

                      -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,

                      -0.5, 0.5f, 0.5f, 0.0f, 1.0f,

                      0.5f, 0.5f, 0.5f, 1.0f, 1.01f,

                      0.5, 0.5f, -0.5f, 1.0f, 0.0f,

                      // bottom
                      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                      -0.5, -0.5f, 0.5f, 0.0f, 1.0f,

                      0.5f, -0.5f, 0.5f, 1.0f, 1.0f,

                      0.5, -0.5f, -0.5f, 1.0f, 0.0f};

  m->verts_size = 24;
  m->verts_cap = 24;
  m->verts = (vertex_s *)alloc_make(sizeof(vertex_s) * m->verts_cap);

  for (int i = 0; i < m->verts_size; i++) {
    m->verts[i].pos.x = vertices[i * 5];
    m->verts[i].pos.y = vertices[i * 5 + 1];
    m->verts[i].pos.z = vertices[i * 5 + 2];
    m->verts[i].texcoord.x = vertices[i * 5 + 3];
    m->verts[i].texcoord.y = vertices[i * 5 + 4];
  }

  uint indices[] = {
      0,  1,  3, // first triangle
      1,  2,  3, // second triangle

      4,  5,  7, // first triangle
      5,  6,  7, // second triangle

      8,  9,  11, // first triangle
      9,  10, 11, // second triangle

      12, 13, 15, // first triangle
      13, 14, 15, // second triangle

      16, 17, 19, // first triangle
      17, 18, 19, // second triangle

      20, 21, 23, // first triangle
      21, 22, 23  // second triangle
  };

  uint faces[] = {
      0,  1,  2, // Front

      2,  3,  0,

      4,  5,  6,  6,  7,  4,

      8,  9,  10, 10, 11, 8,

      12, 13, 14, 14, 15, 12,

      16, 17, 18, 18, 19, 16,

      20, 21, 22, 22, 23, 20,
  };

  m->indices_size = 36;
  m->indices_cap = 36;
  m->indices = (uint *)alloc_make(sizeof(uint) * m->indices_cap);

  for (int i = 0; i < m->indices_size; i++) {
    m->indices[i] = indices[i];
  }

  assert(m->vao == 0);
  assert(m->vbo == 0);
  assert(m->ebo == 0);
}

#endif