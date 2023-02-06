#include "mesh.h"

void mesh_zero(mesh_s *m)  {
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

bool mesh_setup(mesh_s *m) {
  glGenVertexArrays(1, &m->vao);
  glGenBuffers(1, &m->vbo);
  // glGenBuffers(1, &m->ebo);

  glBindVertexArray(m->vao);
  glBindBuffer(GL_ARRAY_BUFFER, m->vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_s) * m->verts_size, m->verts, GL_STATIC_DRAW);

  if (m->indices_size > 0) {
    glGenBuffers(1, &m->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m->indices_size, m->indices, GL_STATIC_DRAW);
  }

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_s), (void*)0);

  // normal attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_s), (void*)offsetof(vertex_s, normal));

  // texcoord attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_s), (void*)offsetof(vertex_s, texcoord));

  glBindVertexArray(0);
  return true;
}

bool mesh_clean(mesh_s *m) {
  glDeleteVertexArrays(1, &m->vao);
  glDeleteBuffers(1, &m->vbo);
  if (m->indices_size > 0) {
    glDeleteBuffers(1, &m->ebo);
  }

  m->verts = (vertex_s*)alloc_free(m->verts);
  m->verts_size = 0;
  m->verts_cap = 0;
  return true;
}

void mesh_draw(mesh_s *m, shader_s *sh) {
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
    sprintf(name, "%s%d", m->textures[i].type, slot);
    shader_1i(sh, name, i);
    glBindTexture(GL_TEXTURE_2D, m->textures[i].id);
  }



  glBindVertexArray(m->vao);
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
  m->textures = (texture_s*)alloc_push(m->textures, &m->textures_size, &m->textures_cap, sizeof(texture_s), &tex);

  return true;
}

bool mesh_read_obj(mesh_s *m, const char * filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
      printf("Failed to open file\n");
      return false;
  }

  printf("File opened\n");

  int i = 0;
  char ch;

  vec3* vertices = (vec3*)alloc_make(16 * sizeof(vec3));
  int vertices_cap = 16;
  int vertices_size = 0;
  vec3* normals = (vec3*)alloc_make(16 * sizeof(vec3));
  int normals_cap = 16;
  int normals_size = 0;
  vec2* texcoords = (vec2*)alloc_make(16 * sizeof(vec2));
  int texcoords_cap = 16;
  int texcoords_size = 0;

  vertex_s *verts = (vertex_s*)alloc_make(16 * sizeof(vertex_s));
  int verts_cap = 16;
  int verts_size = 0;

  while (ch != EOF) {
      char buf[16];
      int n = fscanf(file, "%s", &buf);
      if (n == EOF) {
          printf("fscanf EOF exiting....\n");
          break;
      }
      printf("n: %d\n", n);

      printf("buf: %s\n", buf);
      // (ch = fgetc(file)) != EOF;

      bool next_line = false;

      switch (buf[0]) {
          case '#':
              printf("comment\n");
              next_line = true;
              break;
          case 'o':
              printf("object\n");
              next_line = true;
              break;
          case 'v':
              if (buf[1] == '\0') {
                  if (vertices_size == vertices_cap) {
                      vertices_cap *= 2;
                      vertices = (vec3*)alloc_resize(vertices, vertices_cap * sizeof(vec3));
                  }
                  vec3 *v = &vertices[vertices_size++];
                  fscanf(file, "%f %f %f", &v->x, &v->y, &v->z);
                  printf("v: %f %f %f\n", v->x, v->y, v->z);
                  next_line = true;
              } else if (buf[1] == 't') {
                  if (texcoords_size == texcoords_cap) {
                      texcoords_cap *= 2;
                      texcoords = (vec2*)alloc_resize(texcoords, texcoords_cap * sizeof(vec2));
                  }

                  vec2 *n = &texcoords[texcoords_size++];
                  fscanf(file, "%f %f", &n->x, &n->y);
                  printf("vt: %f %f\n", n->x, n->y);

                  next_line = true;
              } else if (buf[1] == 'n') {
                  if (normals_size == normals_cap) {
                      normals_cap *= 2;
                      normals = (vec3*)alloc_resize(normals, normals_cap * sizeof(vec3));
                  }

                  vec3 *n = &normals[normals_size++];
                  fscanf(file, "%f %f %f", &n->x, &n->y, &n->z);
                  printf("vn: %f %f %f\n", n->x, n->y, n->z);
                  next_line = true;
              }
              break;
          case 'f':
              while (verts_size+3 > verts_cap) {
                  verts_cap *= 2;
                  verts = (vertex_s*)alloc_resize(verts, verts_cap * sizeof(vertex_s));
              }

              unsigned int v1, v2, v3;
              unsigned int vt1, vt2, vt3;
              unsigned int vn1, vn2, vn3;
              fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

              printf("face: %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3);

              verts[verts_size++] = { vertices[v1-1], normals[vn1-1], texcoords[vt1-1] };
              verts[verts_size++] = { vertices[v2-1], normals[vn2-1], texcoords[vt2-1] };
              verts[verts_size++] = { vertices[v3-1], normals[vn3-1], texcoords[vt3-1] };

              next_line = true;
              break;
          default:
              printf("default\n");
              break;
      }

      if (next_line) {
          while ((ch = fgetc(file)) != '\n') {
              if (ch == EOF) {
                  printf("EOF\n");
                  break;
              }
          }
      }

      i++;
  }

  printf("vertices_size: %d\n", vertices_size);
  printf("vertices_cap: %d\n", vertices_cap);
  printf("normals_size: %d\n", normals_size);
  printf("normals_cap: %d\n", normals_cap);
  printf("texcoords_size: %d\n", texcoords_size);
  printf("texcoords_cap: %d\n", texcoords_cap);
  printf("verts_size: %d\n", verts_size);
  printf("verts_cap: %d\n", verts_cap);

  alloc_free(vertices);
  alloc_free(normals);
  alloc_free(texcoords);

  m->verts = verts;
  m->verts_size = verts_size;
  m->verts_cap = verts_cap;

  fclose(file);
  return true;
}