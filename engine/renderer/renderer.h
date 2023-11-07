#ifndef RENDERER_H
#define RENDERER_H

#include "texture.h"

#define MAX_TRIANGLES 2048
#define MAX_VERTICES MAX_TRIANGLES * 3

typedef struct RenderVertex {
  f32 pos[3];
  f32 color[4];
  f32 uv[2];
  f32 texIndex;
} RenderVertex;

typedef struct Renderer {
  // The required OpenGL objects
  u32 vao;
  u32 vbo;
  u32 shader;

  f32 _pj[16];
  m4 projection;
  u32 projectionLocation;

  // Tightly packed triangle data. This is a cpu side mirror of the buffer
  RenderVertex triangleBuffer[MAX_VERTICES];
  u32 triangleLen;

  // Texture stuff
  u32 texture[8];
  u32 textureLen;
} Renderer;

void RenderInit(Renderer *r);
void printShaderLog(u32 shader);
void printProgramLog(u32 program);
void RenderFree(Renderer *r);
void RenderBeginFrame(Renderer *r);
void RenderEndFrame(Renderer *r);

void RenderPushTriangle(Renderer *r, v2 a, v2 b, v2 c, v4 aColor, v4 bColor,
                        v4 cColor, v2 aUV, v2 bUV, v2 cUV, u32 texture);

void RenderPushTriangle3D(Renderer *r, v3 a, v3 b, v3 c, v4 aColor, v4 bColor,
                          v4 cColor, v2 aUV, v2 bUV, v2 cUV, u32 texture);

f32 colorWhite[4] = {1.0f, 1.0f, 1.0f, 1.0f};
f32 colorRed[4] = {1.0f, 0.2f, 0.2f, 1.0f};
f32 colorBlue[4] = {52.0f / 255.0f, 101.0f / 255.0f, 164.0f / 255.0f, 1.0f};

#include "renderer.cpp"
#endif