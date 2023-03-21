#ifndef MAT_TEX_H
#define MAT_TEX_H

#include "shader.h"
#include "unity.h"

struct MaterialTextureSlot {
  GLuint tex;
  GLenum unit;
  int idx;
};

internal MaterialTextureSlot NewMaterialTextureSlot(GLenum unit, int idx) {
  MaterialTextureSlot result = {};

  result.tex = 0;
  result.unit = unit;
  result.idx = idx;

  return result;
}

struct MaterialTexture {
  // GLuint tex_diffuse, tex_specular, tex_emission;
  // GLenum tex_diffuse_unit, tex_specular_unit, tex_emission_unit;
  // int tex_diffuse_unit_idx, tex_specular_unit_idx, tex_emission_unit_idx;

  MaterialTextureSlot diffuse;
  MaterialTextureSlot specular;
  MaterialTextureSlot emission;
};

internal MaterialTexture NewMaterialTexture(MaterialTextureSlot diffuse,
                                            MaterialTextureSlot specular,
                                            MaterialTextureSlot emission) {
  MaterialTexture result = {};

  result.diffuse = diffuse;
  result.specular = specular;
  result.emission = emission;

  return result;
}

void ShaderApplyMaterialTexture(shader_s *shader, MaterialTexture mat) {
  shader_use(shader);

  {
    MaterialTextureSlot tex = mat.diffuse;
    glActiveTexture(tex.unit);
    glBindTexture(GL_TEXTURE_2D, tex.tex);
    shader_1i(shader, "material.diffuse", tex.idx);
  }

  {
    MaterialTextureSlot tex = mat.specular;
    glActiveTexture(tex.unit);
    glBindTexture(GL_TEXTURE_2D, tex.tex);
    shader_1i(shader, "material.specular", tex.idx);
  }

  {
    MaterialTextureSlot tex = mat.emission;
    glActiveTexture(tex.unit);
    glBindTexture(GL_TEXTURE_2D, tex.tex);
    shader_1i(shader, "material.emission", tex.idx);
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

#endif