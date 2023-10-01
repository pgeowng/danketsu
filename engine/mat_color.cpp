#ifndef MAT_COLOR_H
#define MAT_COLOR_H

#include "unity.h"

#include "shader.h"

struct mat_color_s {
  glm::vec3 ambient, diffuse, specular;
  float shininess;
};

mat_color_s newMatColor(glm::vec3 ambient, glm::vec3 diffuse,
                        glm::vec3 specular, float shininess) {
  mat_color_s m;
  m.ambient = ambient;
  m.diffuse = diffuse;
  m.specular = specular;
  m.shininess = shininess;

  return m;
}

global_variable mat_color_s g_mat_sh_0 =
    newMatColor(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f),
                glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);

global_variable mat_color_s g_mat_sh_1 =
    newMatColor(glm::vec3(0.19125f, 0.0735f, 0.0225f),
                glm::vec3(0.7038f, 0.27048f, 0.0828f),
                glm::vec3(0.256777f, 0.137622f, 0.086014f), 12.8f);

global_variable mat_color_s g_mat_sh_2 = newMatColor(
    glm::vec3(0.24725f, 0.1995f, 0.0745f),
    glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.0f, 0.0f, 0.0f), 0.1f);

global_variable mat_color_s g_emerald =
    newMatColor(glm::vec3(0.0215f, 0.1745f, 0.0215f),
                glm::vec3(0.07568f, 0.61424f, 0.07568f),
                glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f * 128.0f);

global_variable mat_color_s g_jade = newMatColor(
    glm::vec3(0.135f, 0.2225f, 0.1575f), glm::vec3(0.54f, 0.89f, 0.63f),
    glm::vec3(0.316228f, 0.316228f, 0.316228f), 0.1f * 128.0f);

global_variable mat_color_s g_obsidian = newMatColor(
    glm::vec3(0.05375f, 0.05f, 0.06625f), glm::vec3(0.18275f, 0.17f, 0.22525f),
    glm::vec3(0.332741f, 0.328634f, 0.346435f), 0.3f * 128.0f);

global_variable mat_color_s g_pearl = newMatColor(
    glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f),
    glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f * 128.0f);
global_variable mat_color_s g_ruby =
    newMatColor(glm::vec3(0.1745f, 0.01175f, 0.01175f),
                glm::vec3(0.61424f, 0.04136f, 0.04136f),
                glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f * 128.0f);

global_variable mat_color_s g_turquoise = newMatColor(
    glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.69102f),
    glm::vec3(0.297254f, 0.30829f, 0.306678f), 0.1f * 128.0f);
global_variable mat_color_s g_brass = newMatColor(
    glm::vec3(0.329412f, 0.223529f, 0.027451f),
    glm::vec3(0.780392f, 0.568627f, 0.113725f),
    glm::vec3(0.992157f, 0.941176f, 0.807843f), 0.21794872f * 128.0f);

global_variable mat_color_s g_bronze = newMatColor(
    glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f),
    glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2f * 128.0f);

global_variable mat_color_s g_chrome =
    newMatColor(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f),
                glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f * 128.0f);

global_variable mat_color_s g_copper =
    newMatColor(glm::vec3(0.19125f, 0.0735f, 0.0225f),
                glm::vec3(0.7038f, 0.27048f, 0.0828f),
                glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f * 128.0f);

global_variable mat_color_s g_gold =
    newMatColor(glm::vec3(0.24725f, 0.1995f, 0.0745f),
                glm::vec3(0.75164f, 0.60648f, 0.22648f),
                glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f * 128.0f);

global_variable mat_color_s g_silver =
    newMatColor(glm::vec3(0.19225f, 0.19225f, 0.19225f),
                glm::vec3(0.50754f, 0.50754f, 0.50754f),
                glm::vec3(0.508273f, 0.508273f, 0.508273f), 0.4f * 128.0f);

global_variable mat_color_s g_black_plastic =
    newMatColor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f),
                glm::vec3(0.50f, 0.50f, 0.50f), 0.25f * 128.0f);

global_variable mat_color_s g_cyan_plastic = newMatColor(
    glm::vec3(0.0f, 0.1f, 0.06f), glm::vec3(0.0f, 0.50980392f, 0.50980392f),
    glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 0.25f * 128.0f);

global_variable mat_color_s g_green_plastic =
    newMatColor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.35f, 0.1f),
                glm::vec3(0.45f, 0.55f, 0.45f), 0.25f * 128.0f);

global_variable mat_color_s g_red_plastic =
    newMatColor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f),
                glm::vec3(0.7f, 0.6f, 0.6f), 0.25f * 128.0f);

global_variable mat_color_s g_white_plastic =
    newMatColor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55f, 0.55f),
                glm::vec3(0.70f, 0.70f, 0.70f), 0.25f * 128.0f);
global_variable mat_color_s g_yellow_plastic =
    newMatColor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f),
                glm::vec3(0.60f, 0.60f, 0.50f), 0.25f * 128.0f);
global_variable mat_color_s g_black_rubber =
    newMatColor(glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.01f, 0.01f, 0.01f),
                glm::vec3(0.4f, 0.4f, 0.4f), 0.078125f * 128.0f);
global_variable mat_color_s g_cyan_rubber =
    newMatColor(glm::vec3(0.0f, 0.05f, 0.05f), glm::vec3(0.4f, 0.5f, 0.5f),
                glm::vec3(0.04f, 0.7f, 0.7f), 0.078125f * 128.0f);
global_variable mat_color_s g_green_rubber =
    newMatColor(glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.4f, 0.5f, 0.4f),
                glm::vec3(0.04f, 0.7f, 0.04f), 0.078125f * 128.0f);
global_variable mat_color_s g_red_rubber =
    newMatColor(glm::vec3(0.05f, 0.0f, 0.0f), glm::vec3(0.5f, 0.4f, 0.4f),
                glm::vec3(0.7f, 0.04f, 0.04f), 0.078125f * 128.0f);

global_variable mat_color_s g_white_rubber =
    newMatColor(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(0.7f, 0.7f, 0.7f), 0.078125f * 128.0f);

global_variable mat_color_s g_yellow_rubber =
    newMatColor(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(0.5f, 0.5f, 0.4f),
                glm::vec3(0.7f, 0.7f, 0.04f), 0.078125f * 128.0f);

global_variable mat_color_s g_mat_color_materials[] = {g_mat_sh_0};
// global_variable mat_color_s g_mat_color_materials[] = {
//   g_mat_sh_0,       g_mat_sh_1,      g_mat_sh_2,      g_emerald,
//   g_jade,           g_obsidian,      g_pearl,         g_ruby,
//   g_turquoise,      g_brass,         g_bronze,        g_chrome,
//   g_copper,         g_gold,          g_silver,        g_black_plastic,
//   g_cyan_plastic,   g_green_plastic, g_red_plastic,   g_white_plastic,
//   g_yellow_plastic, g_black_rubber,  g_cyan_rubber,   g_green_rubber,
//   g_red_rubber,     g_white_rubber,  g_yellow_rubber,
// };

internal void mat_color_apply(mat_color_s mat, shader_s *shader) {
  shader_3f(shader, "material.ambient", mat.ambient.r, mat.ambient.g,
            mat.ambient.b);
  shader_3f(shader, "material.diffuse", mat.diffuse.r, mat.diffuse.g,
            mat.diffuse.b);
  shader_3f(shader, "material.specular", mat.specular.r, mat.specular.g,
            mat.specular.b);
  // shader_Set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
  shader_1f(shader, "material.shininess", mat.shininess);
}
#endif