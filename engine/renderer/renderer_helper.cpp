#include "base_types.h"

#include "renderer.h"
#include "renderer_helper.h"

void RenderExInit(RendererEx *r) {
  RenderInit(&r->r);

  r->cullingQuad[2] = 1000.0f;
  r->cullingQuad[3] = 1000.0f;
}

u32 _cached_white = 4096;

u32 RenderWhiteTexture() {
  if (_cached_white == 4096) {
    u32 tex;
    u8 image[4] = {255, 255, 255, 255};
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    _cached_white = tex;
  }
  return _cached_white;
}

void RenderExFree(RendererEx *r) {
  RenderTextureFree(_cached_white);
  RenderFree(&r->r);
}

void RenderPushQuadColor(RendererEx *r, rect quad, v4 color) {

  u32 texture = RenderWhiteTexture();
  f32 a[2] = {quad[0], quad[1]};
  f32 aUV[2] = {0, 0};
  f32 b[2] = {quad[0] + quad[2], quad[1]};
  f32 bUV[2] = {1, 0};
  f32 c[2] = {quad[0], quad[1] + quad[3]};
  f32 cUV[2] = {0, 1};
  f32 d[2] = {quad[0] + quad[2], quad[1] + quad[3]};
  f32 dUV[2] = {1, 1};

  RenderPushTriangle(&r->r, v2Clamp(a, r->cullingQuad),
                     v2Clamp(b, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, aUV, bUV, cUV, texture);
  RenderPushTriangle(&r->r, v2Clamp(b, r->cullingQuad),
                     v2Clamp(d, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, bUV, dUV, cUV, texture);
}

void RenderPushQuadTex(RendererEx *r, rect quad, u32 texture, v4 color) {
  f32 uv[4] = {0.0f, 0.0f, 1.0f, 1.0f};
  rectUVCull(uv, quad, r->cullingQuad);

  f32 a[2] = {quad[0], quad[1]};
  f32 aUV[2] = {uv[0], uv[1]};
  f32 b[2] = {quad[0] + quad[2], quad[1]};
  f32 bUV[2] = {uv[0] + uv[2], uv[1]};
  f32 c[2] = {quad[0], quad[1] + quad[3]};
  f32 cUV[2] = {uv[0], uv[1] + uv[3]};
  f32 d[2] = {quad[0] + quad[2], quad[1] + quad[3]};
  f32 dUV[2] = {uv[0] + uv[2], uv[1] + uv[3]};

  RenderPushTriangle(&r->r, v2Clamp(a, r->cullingQuad),
                     v2Clamp(b, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, aUV, bUV, cUV, texture);
  RenderPushTriangle(&r->r, v2Clamp(b, r->cullingQuad),
                     v2Clamp(d, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, bUV, dUV, cUV, texture);
}

void RenderPushQuadSubTex(RendererEx *r, rect quad, rect uvQuad, u32 texture,
                          v4 color) {
  f32 uv[4] = {uvQuad[0], uvQuad[1], uvQuad[2], uvQuad[3]};
  rectUVCull(uv, quad, r->cullingQuad);

  f32 a[2] = {quad[0], quad[1]};
  f32 aUV[2] = {uv[0], uv[1]};
  f32 b[2] = {quad[0] + quad[2], quad[1]};
  f32 bUV[2] = {uv[0] + uv[2], uv[1]};
  f32 c[2] = {quad[0], quad[1] + quad[3]};
  f32 cUV[2] = {uv[0], uv[1] + uv[3]};
  f32 d[2] = {quad[0] + quad[2], quad[1] + quad[3]};
  f32 dUV[2] = {uv[0] + uv[2], uv[1] + uv[3]};

  RenderPushTriangle(&r->r, v2Clamp(a, r->cullingQuad),
                     v2Clamp(b, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, aUV, bUV, cUV, texture);
  RenderPushTriangle(&r->r, v2Clamp(b, r->cullingQuad),
                     v2Clamp(d, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, bUV, dUV, cUV, texture);
}