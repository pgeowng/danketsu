#ifndef OBJ_READER_H
#define OBJ_READER_H

#include "memory/base_memory.h"
#include "memory/base_memory_malloc.cpp"
#include "mesh.h"
#include "obj/mtl_reader.cpp"

bool mesh_read_obj(mesh_s *m, MArena *arena, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open file\n");
    return false;
  }

  char *matPath = 0;

  printf("File opened\n");

  int i = 0;
  char ch;

  vec3 *vertices = (vec3 *)alloc_make(16 * sizeof(vec3));
  int vertices_cap = 16;
  int vertices_size = 0;
  vec3 *normals = (vec3 *)alloc_make(16 * sizeof(vec3));
  int normals_cap = 16;
  int normals_size = 0;
  vec2 *texcoords = (vec2 *)alloc_make(16 * sizeof(vec2));
  int texcoords_cap = 16;
  int texcoords_size = 0;

  vertex_s *verts = (vertex_s *)alloc_make(16 * sizeof(vertex_s));
  int verts_cap = 16;
  int verts_size = 0;

  while (ch != EOF) {
    char buf[16];
    int n = fscanf(file, "%s", &buf);
    if (n == EOF) {
      printf("fscanf EOF exiting....\n");
      break;
    }

    printf("dir=%s ", buf);
    // (ch = fgetc(file)) != EOF;

    bool next_line = false;

    if (strcmp(buf, "mtllib") == 0 && matPath == 0) {
      printf("mtllib token: \n");

      MTemp mem = MTempBegin(arena);
      char *matFilename = PushArray(mem.arena, char, 32);

      int n = fscanf(file, "%s", matFilename);
      if (n == EOF) {
        printf("read mtlib next token failed\n");
        break;
      }

      strcpy(matPath, buf);
      printf("material file: %s\n", matPath);
      printf("filename: %s\n", filename);

      char *folderName = PushArray(mem.arena, char, 32);
      {
        bool ok = FSDirname(folderName, filename);
        if (!ok) {
          printf("WaveObjReadOBJ failed: can't get dir name for %s\n",
                 filename);
          return false;
        }
      }

      char *mtlPath = PushArray(mem.arena, char, 32);
      {
        bool ok = FSJoin(mtlPath, folderName, matFilename);
        if (!ok) {
          printf("WaveObjReadOBJ failed: can't FSJoin folderName=%s and "
                 "mtlFilename=%s\n",
                 folderName, matFilename);
          return false;
        }
      }

      WaveMTL mtl = {0};

      WaveObjReadMTL(arena, mtlPath, &mtl);
    }

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
          vertices =
              (vec3 *)alloc_resize(vertices, vertices_cap * sizeof(vec3));
        }
        vec3 *v = &vertices[vertices_size++];
        fscanf(file, "%f %f %f", &v->x, &v->y, &v->z);
        printf("v: %f %f %f\n", v->x, v->y, v->z);
        next_line = true;
      } else if (buf[1] == 't') {
        if (texcoords_size == texcoords_cap) {
          texcoords_cap *= 2;
          texcoords =
              (vec2 *)alloc_resize(texcoords, texcoords_cap * sizeof(vec2));
        }

        vec2 *n = &texcoords[texcoords_size++];
        fscanf(file, "%f %f", &n->x, &n->y);
        printf("vt: %f %f\n", n->x, n->y);

        next_line = true;
      } else if (buf[1] == 'n') {
        if (normals_size == normals_cap) {
          normals_cap *= 2;
          normals = (vec3 *)alloc_resize(normals, normals_cap * sizeof(vec3));
        }

        vec3 *n = &normals[normals_size++];
        fscanf(file, "%f %f %f", &n->x, &n->y, &n->z);
        printf("vn: %f %f %f\n", n->x, n->y, n->z);
        next_line = true;
      }
      break;
    case 'f': {
      int n = 0;
      uint v1, vt1, vn1;
      uint v2, vt2, vn2;
      uint v3, vt3, vn3;

      n = fscanf(file, "%d/%d/%d", &v1, &vt1, &vn1);
      if (n != 3) {
        LOG_ERROR("mesh_read_obj: failed to read first pair of the face");
        break;
      }

      n = fscanf(file, "%d/%d/%d", &v2, &vt2, &vn2);
      if (n != 3) {
        LOG_ERROR("mesh_read_obj: failed to read second pair of the face");
        break;
      }

      n = fscanf(file, "%d/%d/%d", &v3, &vt3, &vn3);
      if (n != 3) {
        LOG_ERROR("mesh_read_obj: failed to read third pair of the face");
        break;
      }

      while (n == 3) {

        while (verts_size + 3 > verts_cap) {
          verts_cap *= 2;
          verts = (vertex_s *)alloc_resize(verts, verts_cap * sizeof(vertex_s));
        }

        printf("%d/%d/%d %d/%d/%d %d/%d/%d\n", v1, vt1, vn1, v2, vt2, vn2, v3,
               vt3, vn3);

        verts[verts_size++] = {vertices[v1 - 1], normals[vn1 - 1],
                               texcoords[vt1 - 1]};
        verts[verts_size++] = {vertices[v2 - 1], normals[vn2 - 1],
                               texcoords[vt2 - 1]};
        verts[verts_size++] = {vertices[v3 - 1], normals[vn3 - 1],
                               texcoords[vt3 - 1]};

        // moving last vertex to the previous slot
        v2 = v3;
        vt2 = vt3;
        vn2 = vn3;

        n = fscanf(file, "%d/%d/%d", &v3, &vt3, &vn3);
        if (n != 3) {
          LOG_ERROR("mesh_read_obj: failed to read new pair of the face");
        }
      }

      LOG_DEBUG("mesh_read_obj: face read finished");

      next_line = false;
      break;
    }
    default:
      printf("default\n");
      break;
    }

    if (next_line) {
      while ((ch = fgetc(file)) != '\n') {
        printf("skipping char: '%c'\n", ch);
        if (ch == EOF) {
          LOG_DEBUG("mesh_read_obj: EOF\n");
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

internal bool FSDirname(char *dst, const char *fpath) {
  if (fpath == NULL || dst == NULL) {
    return false;
  }

  const char *lastChar = fpath;
  const char *sepChar = NULL;
  while (*lastChar != '\0') {
    if (*lastChar == '/') {
      sepChar = lastChar;
    }
    lastChar++;
  }

  // NOTE: passed just filename: hello.h -> ./
  if (sepChar == NULL) {
    strcpy(dst, "./");
    return true;
  }

  // NOTE: taking /a/b/tcs -> /a/b/
  U64 l = sepChar - fpath;
  memcpy(dst, fpath, l);
  dst[l + 1] = '\0';

  return true;
}

internal bool FSJoin(char *dst, const char *fpath, const char *rpath) {
  if (dst == NULL || fpath == NULL || rpath == NULL) {
    return false;
  }

  U64 l = strlen(fpath);
  memcpy(dst, fpath, l);

  U64 l2 = strlen(rpath);
  memcpy(dst + l, rpath, l2);

  *(dst + l + l2 + 1) = '\0';

  return true;
}

#endif