#ifndef EXAMPLE_RAMP_MESH_H
#define EXAMPLE_RAMP_MESH_H

#include "alloc.h"
#include "mesh.h"

void mesh_init_ramp(mesh_s *m) {
  float x_45deg = 0.70710678118f;

  float ramp[] = {
    // up
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    // up right
    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    x_45deg,
    -x_45deg,

    // left
    -0.5f,
    -0.5f,
    -0.5f,
    -1.0f,
    0.0f,
    0.0f,

    -0.5f,
    -0.5f,
    0.5f,
    -1.0f,
    0.0f,
    0.0f,

    -0.5f,
    0.5f,
    0.5f,
    -1.0f,
    0.0f,
    0.0f,

    // right
    0.5f,
    -0.5f,
    -0.5f,
    1.0f,
    0.0f,
    0.0f,

    0.5f,
    -0.5f,
    0.5f,
    1.0f,
    0.0f,
    0.0f,

    0.5f,
    0.5f,
    0.5f,
    1.0f,
    0.0f,
    0.0f,

    // bottom
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,

    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,

    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,

    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,

    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,

    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,

    // back
    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,
  };

  assert(m->verts == NULL);
  m->verts_size = 36;
  m->verts_cap = 36;
  m->verts = (vertex_s *)alloc_make(sizeof(vertex_s) * m->verts_cap);

  for (int i = 0; i < m->verts_size; i++) {
    log_debug("i: %d", i*6+5);
    m->verts[i].pos.x = ramp[i * 6 + 0];
    m->verts[i].pos.y = ramp[i * 6 + 1];
    m->verts[i].pos.z = ramp[i * 6 + 2];
    m->verts[i].normal.x = ramp[i * 6 + 3];
    m->verts[i].normal.y = ramp[i * 6 + 4];
    m->verts[i].normal.z = ramp[i * 6 + 5];
  }

  assert(m->indices == NULL);
  m->indices = NULL;
  m->indices_cap = 0;
  m->indices_size = 0;
}

#endif