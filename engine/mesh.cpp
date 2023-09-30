#include "mesh.h"

void MeshZero(mesh_s *m) {
  m->verts = NULL;
  m->verts_size = 0;
  m->verts_cap = 0;

  m->indices = NULL;
  m->indices_size = 0;
  m->indices_cap = 0;

  m->textures = NULL;
  m->textures_size = 0;
  m->textures_cap = 0;

  m->vao = 0;
  m->vbo = 0;
  m->ebo = 0;
}

void MeshCheckClean(mesh_s *m) {
  assert(m->verts == NULL);
  assert(m->verts_size == 0);
  assert(m->verts_cap == 0);
  assert(m->indices == NULL);
  assert(m->indices_size == 0);
  assert(m->indices_cap == 0);
  assert(m->textures == NULL);
  assert(m->textures_size == 0);
  assert(m->textures_cap == 0);
  assert(m->vao == 0);
  assert(m->vbo == 0);
  assert(m->ebo == 0);
}

bool MeshClean(mesh_s *m) {
  glDeleteVertexArrays(1, &m->vao);
  m->vao = 0;

  glDeleteBuffers(1, &m->vbo);
  m->vbo = 0;

  if (m->ebo != 0) {
    glDeleteBuffers(1, &m->ebo);
    m->ebo = 0;
  }

  alloc_free(m->verts);
  m->verts = NULL;
  m->verts_size = 0;
  m->verts_cap = 0;

  alloc_free(m->indices);
  m->indices = NULL;
  m->indices_size = 0;
  m->indices_cap = 0;

  alloc_free(m->textures);
  m->textures = NULL;
  m->textures_size = 0;
  m->textures_cap = 0;

  return true;
}

bool MeshInitialize(mesh_s *m) {
  glGenVertexArrays(1, &m->vao);
  glGenBuffers(1, &m->vbo);
  // glGenBuffers(1, &m->ebo);

  glBindVertexArray(m->vao);
  glBindBuffer(GL_ARRAY_BUFFER, m->vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_s) * m->verts_size, m->verts,
               GL_STATIC_DRAW);

  if (m->indices_size > 0) {
    glGenBuffers(1, &m->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m->indices_size,
                 m->indices, GL_STATIC_DRAW);
  }

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_s), (void *)0);

  // normal attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_s),
                        (void *)offsetof(vertex_s, normal));

  // texcoord attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_s),
                        (void *)offsetof(vertex_s, texcoord));

  glBindVertexArray(0);
  return true;
}

void MeshDraw(mesh_s *m, shader_s *sh) {
  int diffuse_nr = 1;
  int specular_nr = 1;

  for (int i = 0; i < m->textures_size; i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    int slot = 0;
    if (strcmp(m->textures[i].type, "material.diffuse") == 0) {
      slot = diffuse_nr++;
    } else if (strcmp(m->textures[i].type, "material.specular") == 0) {
      slot = specular_nr++;
    }

    char name[50];
    mysprintf(name, "%s%d", m->textures[i].type, slot);
    shader_1i(sh, name, i);
    glBindTexture(GL_TEXTURE_2D, m->textures[i].id);
  }

  glBindVertexArray(m->vao);
  // printf("MeshDraw: indices_size: %d\n", m->indices_size);
  // printf("MeshDraw: verts_size: %d\n", m->verts_size);
  assert(m->verts_size != 0);
  if (m->indices_size > 0) {
    glDrawElements(GL_TRIANGLES, m->indices_size, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, m->verts_size);
  }
  glBindVertexArray(0);

  glActiveTexture(GL_TEXTURE0);
}

bool mesh_add_texture(mesh_s *m, const char *path, const char *type) {
  texture_s tex;
  bool ok = tex_load(&tex.id, path);
  if (!ok) {
    printf("mesh_add_texture: failed to load texture %s", path);
    return false;
  }
  tex.type = type;
  // tex.path = path;
  m->textures =
      (texture_s *)alloc_push(m->textures, &m->textures_size, &m->textures_cap,
                              sizeof(texture_s), &tex);

  return true;
}
