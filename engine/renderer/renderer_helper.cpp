#include "base_types.h"

#include "renderer.h"
#include "renderer_helper.h"

void RenderExInit(RendererEx *r) {
  RenderInit(&r->r);

  r->cullingQuad[2] = 1000.0f;
  r->cullingQuad[3] = 1000.0f;

  v4Copy((v4)&r->defaultColor, colorWhite);
}

void RenderSetTileFont(RendererEx *r, TileMap *font) { r->font = font; }

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

void RenderPush3DQuadColor(RendererEx *r, v3 a, v3 b, v3 c, v3 d, v4 color) {
  u32 texture = RenderWhiteTexture();
  f32 aUV[2] = {0, 0};
  f32 bUV[2] = {1, 0};
  f32 cUV[2] = {0, 1};
  f32 dUV[2] = {1, 1};

  RenderPushTriangle3D(&r->r, a, b, c, color, color, color, aUV, bUV, cUV,
                       texture);
  RenderPushTriangle3D(&r->r, b, d, c, color, color, color, bUV, dUV, cUV,
                       texture);
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

// NOTE: from left to right, from up to bottom.
void RenderPushAtlasTile(RendererEx *r, rect position, TileMap *tm, u32 index,
                         v4 color) {

  f32 uvQuad[4] = {0.0f};
  TileMapUVByIdx(uvQuad, tm, index);

  RenderPushQuadSubTex(r, position, uvQuad, tm->texture->texture, color);
}

void RenderPushString(RendererEx *r, rect position, const char *s) {
  assert(r->font != 0);
  f32 glyphWidth = position[3] / r->font->tileHeight * r->font->tileWidth;
  f32 pos[4] = {position[0], position[1], glyphWidth, position[3]};

  for (u32 i = 0; s[i] != '\0'; i++) {
    char c = s[i];
    u32 index = c - 32;

    RenderPushAtlasTile(r, pos, r->font, index, r->defaultColor);
    pos[0] += glyphWidth;
  }
}

void RenderPushQuadSubTexOrigRot(RendererEx *r, rect quad, rect uvQuad,
                                 u32 texture, v4 color, v2 origin,
                                 f32 rotation) {

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

  f32 rot[4] = {0.0f};
  m2Rotate(rot, rotation);

  v2Subtract(a, quad);
  v2Subtract(b, quad);
  v2Subtract(c, quad);
  v2Subtract(d, quad);

  v2Subtract(a, origin);
  v2Subtract(b, origin);
  v2Subtract(c, origin);
  v2Subtract(d, origin);

  m2MultiplyPoint(rot, a);
  m2MultiplyPoint(rot, b);
  m2MultiplyPoint(rot, c);
  m2MultiplyPoint(rot, d);

  v2Add(a, origin);
  v2Add(b, origin);
  v2Add(c, origin);
  v2Add(d, origin);

  v2Add(a, quad);
  v2Add(b, quad);
  v2Add(c, quad);
  v2Add(d, quad);

  RenderPushTriangle(&r->r, v2Clamp(a, r->cullingQuad),
                     v2Clamp(b, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, aUV, bUV, cUV, texture);
  RenderPushTriangle(&r->r, v2Clamp(b, r->cullingQuad),
                     v2Clamp(d, r->cullingQuad), v2Clamp(c, r->cullingQuad),
                     color, color, color, bUV, dUV, cUV, texture);
}

#define DEBUG_POINT_CAP 20
static u32 debugPointOffset = 4;
static f32 debugPointQuadBuffer[DEBUG_POINT_CAP * 4];
static f32 debugPointColorBuffer[DEBUG_POINT_CAP * 4];
static u32 debugPointLen = 0;
static u32 debugPointStart = 0;

static void RenderPushDebugPoint(RendererEx *r, rect quad, v4 color) {
  v4Copy(&debugPointQuadBuffer[debugPointOffset * debugPointStart], quad);
  v4Copy(&debugPointColorBuffer[debugPointOffset * debugPointStart], color);

  if (debugPointLen == DEBUG_POINT_CAP) {
    debugPointStart = (debugPointStart + 1) % DEBUG_POINT_CAP;
  } else {
    debugPointLen++;
  }
}

static void RenderDebugPoints(RendererEx *r) {
  assert(r->font != 0);
  f32 textPos[4] = {0.0f, 64.0f, 100.0f, 32.0f};
  char buf[20];
  sprintf_s(buf, "idx: %d, %d", debugPointStart, debugPointLen);
  RenderPushString(r, textPos, buf);

  for (u32 i = 0; i < debugPointLen; i++) {
    u32 offset = (debugPointStart + i) % DEBUG_POINT_CAP * debugPointOffset;
    (&debugPointColorBuffer[offset])[3] = (f32)(i) / DEBUG_POINT_CAP;
    RenderPushQuadColor(r, &debugPointQuadBuffer[offset],
                        &debugPointColorBuffer[offset]);
  }
}