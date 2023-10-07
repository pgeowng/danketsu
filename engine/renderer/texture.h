#ifndef TEXTURER_H
#define TEXTURER_H

typedef struct Texture {
  u32 texture;
  u32 width;
  u32 height;
  u32 channels;
} Texture;

#include "texture.cpp"

#endif