#ifndef MAT_TEX_H
#define MAT_TEX_H

#include "shader.h"
#include "unity.h"

struct mat_tex_s {
  GLuint tex_diffuse, tex_specular, tex_emission;
  GLenum tex_diffuse_unit, tex_specular_unit, tex_emission_unit;
  int tex_diffuse_unit_idx, tex_specular_unit_idx, tex_emission_unit_idx;

  float shininess;
};

void mat_tex_apply(mat_tex_s mat, shader_s* shader) {
  shader_use(shader);
  glActiveTexture(mat.tex_diffuse_unit);
  glBindTexture(GL_TEXTURE_2D, mat.tex_diffuse);
  shader_1i(shader, "material.diffuse", mat.tex_diffuse_unit_idx);

  glActiveTexture(mat.tex_specular_unit);
  glBindTexture(GL_TEXTURE_2D, mat.tex_specular);
  shader_1i(shader, "material.specular", mat.tex_specular_unit_idx);

  glActiveTexture(mat.tex_emission_unit);
  glBindTexture(GL_TEXTURE_2D, mat.tex_emission);
  shader_1i(shader, "material.emission", mat.tex_emission_unit_idx);

  shader_1f(shader, "material.shininess", mat.shininess);

  shader_1f(shader, "time", SDL_GetTicks() / 1000.0f);
}

#endif