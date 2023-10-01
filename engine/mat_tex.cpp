#ifndef MAT_TEX_H
#define MAT_TEX_H

#include "shader.h"
#include "unity.h"

enum MaterialTextureSlotKind {
  None = 0,
  Diffuse = 10,
  Specular = 20,
  Emission = 30
};

struct MaterialTextureSlot {
  MaterialTextureSlotKind kind = MaterialTextureSlotKind::None;
  GLuint tex;
  GLenum unit;
  int idx;
};

internal MaterialTextureSlot
NewMaterialTextureSlot(MaterialTextureSlotKind kind, GLenum unit, int idx) {
  MaterialTextureSlot result = {};

  result.tex = 0;
  result.kind = kind;
  result.unit = unit;
  result.idx = idx;

  return result;
}

struct MaterialTexture {
  MaterialTextureSlot slots[3];
};

internal MaterialTexture NewMaterialTexture(MaterialTextureSlot diffuse,
                                            MaterialTextureSlot specular,
                                            MaterialTextureSlot emission) {
  MaterialTexture result = {};

  result.slots[0] = diffuse;
  result.slots[1] = specular;
  result.slots[2] = emission;

  return result;
}

void ShaderApplyMaterialTexture(shader_s *shader, MaterialTexture mat) {
  shader_use(shader);

  {
    MaterialTextureSlot tex = mat.slots[0];
    glActiveTexture(tex.unit);
    glBindTexture(GL_TEXTURE_2D, tex.tex);
    shader_1i(shader, "material.diffuseTex1", tex.idx);
  }

  {
    MaterialTextureSlot tex = mat.slots[1];
    glActiveTexture(tex.unit);
    glBindTexture(GL_TEXTURE_2D, tex.tex);
    shader_1i(shader, "material.specularTex1", tex.idx);
  }

  {
    MaterialTextureSlot tex = mat.slots[2];
    glActiveTexture(tex.unit);
    glBindTexture(GL_TEXTURE_2D, tex.tex);
    shader_1i(shader, "material.emissionTex1", tex.idx);
  }

  // TODO: dispose
  shader_1f(shader, "time", SDL_GetTicks() / 1000.0f);
}

void ShaderApplyMaterialColor(shader_s *shader, mat_color_s mat) {
  {
    vec3 c = mat.ambient;
    shader_3f(shader, "material.ambientColor", c.r, c.g, c.b);
  }

  {
    vec3 c = mat.diffuse;
    shader_3f(shader, "material.diffuseColor", c.r, c.g, c.b);
  }

  {
    vec3 c = mat.specular;
    shader_3f(shader, "material.specularColor", c.r, c.g, c.b);
  }

  shader_1f(shader, "material.shininess", mat.shininess);
}

void ShaderApplyMatLight(shader_s *shader, mat_color_s mat, light_s light) {
  {
    vec3 c = mat.ambient;
    shader_3f(shader, "material.ambient", c.r, c.g, c.b);
  }

  {
    vec3 c = mat.diffuse;
    shader_3f(shader, "material.diffuse", c.r, c.g, c.b);
  }

  {
    vec3 c = mat.specular;
    shader_3f(shader, "material.specular", c.r, c.g, c.b);
  }

  shader_1f(shader, "material.shininess", mat.shininess);

  {
    vec3 c = light.position;
    shader_3f(shader, "light.position", c.r, c.g, c.b);
  }

  {
    vec3 c = light.ambient;
    shader_3f(shader, "light.ambient", c.r, c.g, c.b);
  }

  {
    vec3 c = light.diffuse;
    shader_3f(shader, "light.diffuse", c.r, c.g, c.b);
  }

  {
    vec3 c = light.specular;
    shader_3f(shader, "light.specular", c.r, c.g, c.b);
  }
}

#endif