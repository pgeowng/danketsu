#ifndef RENDER_HELP_H
#define RENDER_HELP_H

typedef struct RendererEx {
  Renderer r;
  f32 cullingQuad[4];
} RendererEx;

void RenderExInit(RendererEx *r);
void RenderPushQuatColor(RendererEx *r, rect quad, v4 color);
u32 RenderWhiteTexture();

#include "renderer_helper.cpp"

#endif