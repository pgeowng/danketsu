#ifndef RENDERER_H
#define RENDERER_H

typedef struct RenderVertex {
  vec2 pos;
  vec4 color;
} RenderVertex;

typedef struct Renderer {
  // The required OpenGL objects
  u32 vao;
  u32 vbo;
  u32 shader;

  mat4 projection;

  // Tightly packed triangle data. This is a cpu side mirror of the buffer
  RenderVertex triangleData[MAX_VERTICES];
  u32 triangleLen;
} Renderer;

void RenderInit(Renderer *r);

#endif