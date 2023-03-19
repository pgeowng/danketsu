#ifndef EXAMPLE_CUBE_TEX_MESH_H
#define EXAMPLE_CUBE_TEX_MESH_H

#include "alloc.h"
#include "mesh.h"

void MeshSetCubeTextured(mesh_s *m) {
  assert(m->vao == 0);
  assert(m->vbo == 0);
  assert(m->ebo == 0);

  float cube[] = {// front
                  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,

                  -0.5f, 0.5f, -0.5f, // Front Left Top
                  0.0f, 0.0f, -0.5f, 0.0f, 1.0f,

                  0.5f, 0.5f, -0.5f, // Front Right Top
                  0.0f, 0.0f, -0.5f, 1.0f, 1.0f,

                  0.5f, 0.5f, -0.5f, // Front Right Top
                  0.0f, 0.0f, -0.5f, 1.0f, 1.0f,

                  0.5f, -0.5f, -0.5f, // Front Right Bottom
                  0.0f, 0.0f, -0.5f, 1.0f, 0.0f,

                  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,

                  // right
                  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                  0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

                  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                  0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                  // back
                  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

                  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

                  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                  // left
                  -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

                  -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                  -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                  -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                  // top
                  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                  -0.5, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

                  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                  0.5, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

                  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                  // bottom
                  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

                  -0.5, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

                  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

                  0.5, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

                  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f};

  assert(m->verts == NULL);
  m->verts_size = 36;
  m->verts_cap = 36;
  m->verts = (vertex_s *)alloc_make(sizeof(vertex_s) * m->verts_cap);

  for (int i = 0; i < m->verts_size; i++) {
    m->verts[i].pos = (vec3){cube[i * 8 + 0], cube[i * 8 + 1], cube[i * 8 + 2]};
    m->verts[i].normal =
        (vec3){cube[i * 8 + 3], cube[i * 8 + 4], cube[i * 8 + 5]};
    m->verts[i].texcoord = (vec2){cube[i * 8 + 6], cube[i * 8 + 7]};
  }

  assert(m->indices == NULL);
  m->indices = NULL;
  m->indices_size = 0;
  m->indices_cap = 0;
}

#endif