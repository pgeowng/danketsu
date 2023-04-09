#ifndef OBJ_H
#define OBJ_H
#include "memory/base_memory.h"
#include "unity.h"

struct WaveMTL {
  float specularExponent;
  vec3 ambientColor;
  vec3 diffuseColor;
  vec3 specularColor;
  vec3 emissiveColor;
  float opticalDensity;
  float transparency;
  int illum;

  char *diffuseTexPath = 0;
};

internal bool WaveObjReadMTL(MArena *arena, const char *fpath, WaveMTL);

internal bool readToken(FILE *file, char *buf);
internal bool readFloat(FILE *file, float *v);
internal bool readVec3(FILE *file, vec3 *v);
internal bool readInt(FILE *file, int *v);

internal bool FSDirname(char *dst, const char *fpath);
internal bool FSJoin(char *dst, const char *fpath, const char *rpath);

#endif