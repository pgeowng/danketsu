#ifndef MTL_READER_H
#define MTL_READER_H
#include "memory/base_memory.h"
#include "obj/obj.h"

internal bool WaveObjReadMTL(MArena *arena, const char *fpath, WaveMTL *mtl) {
  FILE *file = fopen(fpath, "r");
  if (file == NULL) {
    printf("WaveObjReadMTL failed: can't open file '%s'", fpath);
    return false;
  }

  assert(mtl->diffuseTexPath == 0);
  *mtl = {0};

  char ch = 1;
  while (ch != EOF) {
    char buf[16];
    bool ok = readToken(file, &buf[0]);
    if (!ok) {
      break;
    }

    if (strcmp(buf, "Ns") == 0) {
      bool ok = readFloat(file, &mtl->specularExponent);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read float after Ns");
        return false;
      }
    } else if (strcmp(buf, "Ka") == 0) {
      bool ok = readVec3(file, &mtl->ambientColor);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read vec3 after Ka");
        return false;
      }
    } else if (strcmp(buf, "Kd") == 0) {
      bool ok = readVec3(file, &mtl->diffuseColor);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read vec3 after Kd");
        return false;
      }
    } else if (strcmp(buf, "Ks") == 0) {
      bool ok = readVec3(file, &mtl->specularColor);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read vec3 after Ks");
        return false;
      }
    } else if (strcmp(buf, "Ke") == 0) {
      bool ok = readVec3(file, &mtl->emissiveColor);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read vec3 after Ke");
        return false;
      }
    } else if (strcmp(buf, "Ni") == 0) {
      bool ok = readFloat(file, &mtl->opticalDensity);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read float after Ni");
        return false;
      }
    } else if (strcmp(buf, "d") == 0) {
      bool ok = readFloat(file, &mtl->transparency);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read float after d");
        return false;
      }
    } else if (strcmp(buf, "illum") == 0) {
      bool ok = readInt(file, &mtl->illum);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read int after illum");
        return false;
      }
    } else if (strcmp(buf, "map_Kd") == 0) {
      bool ok = readToken(file, &buf[0]);
      if (!ok) {
        printf("WaveObjReadMTL failed: can't read string after map_Kd");
        return false;
      }

      mtl->diffuseTexPath = PushArray(arena, char, 16);
    }
  }

  return true;
}

internal bool readToken(FILE *file, char *buf) {
  int n = fscanf(file, "%s", buf);
  return n == 1;
}

internal bool readFloat(FILE *file, float *v) {
  int n = fscanf(file, "%f", v);
  return n == 1;
}

internal bool readVec3(FILE *file, vec3 *v) {
  int n = fscanf(file, "%f %f %f", &v->x, &v->y, &v->z);
  return n == 3;
}

internal bool readInt(FILE *file, int *v) {
  int n = fscanf(file, "%d", v);
  return n == 1;
}

#endif