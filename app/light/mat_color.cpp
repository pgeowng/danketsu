#ifndef MAT_COLOR_H
#define MAT_COLOR_H

#include "unity.h"

#include "shader.h"

struct mat_color_s {
  glm::vec3 ambient, diffuse, specular;
  float shininess;
};

global_variable mat_color_s g_mat_sh_0 = {
    .ambient = glm::vec3(1.0f, 0.5f, 0.31f),
    .diffuse = glm::vec3(1.0f, 0.5f, 0.31f),
    .specular = glm::vec3(0.5f, 0.5f, 0.5f),
    .shininess = 32.0f,
};

global_variable mat_color_s g_mat_sh_1 = {
    .ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f),
    .diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f),
    .specular = glm::vec3(0.256777f, 0.137622f, 0.086014f),
    .shininess = 12.8f,
};

global_variable mat_color_s g_mat_sh_2 = {
    .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
    .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f),
    .specular = glm::vec3(0.0f, 0.0f, 0.0f),
    .shininess = 0.1f,
};

global_variable mat_color_s g_emerald = {
    .ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f),
    .diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f),
    .specular = glm::vec3(0.633f, 0.727811f, 0.633f),
    .shininess = 0.6f * 128.0f,
};

global_variable mat_color_s g_jade = {
    .ambient = glm::vec3(0.135f, 0.2225f, 0.1575f),
    .diffuse = glm::vec3(0.54f, 0.89f, 0.63f),
    .specular = glm::vec3(0.316228f, 0.316228f, 0.316228f),
    .shininess = 0.1f * 128.0f,
};

global_variable mat_color_s g_obsidian = {
    .ambient = glm::vec3(0.05375f, 0.05f, 0.06625f),
    .diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f),
    .specular = glm::vec3(0.332741f, 0.328634f, 0.346435f),
    .shininess = 0.3f * 128.0f,
};

global_variable mat_color_s g_pearl = {
    .ambient = glm::vec3(0.25f, 0.20725f, 0.20725f),
    .diffuse = glm::vec3(1.0f, 0.829f, 0.829f),
    .specular = glm::vec3(0.296648f, 0.296648f, 0.296648f),
    .shininess = 0.088f * 128.0f,
};
global_variable mat_color_s g_ruby = {
    .ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f),
    .diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f),
    .specular = glm::vec3(0.727811f, 0.626959f, 0.626959f),
    .shininess = 0.6f * 128.0f,
};

global_variable mat_color_s g_turquoise = {
    .ambient = glm::vec3(0.1f, 0.18725f, 0.1745f),
    .diffuse = glm::vec3(0.396f, 0.74151f, 0.69102f),
    .specular = glm::vec3(0.297254f, 0.30829f, 0.306678f),
    .shininess = 0.1f * 128.0f,
};
global_variable mat_color_s g_brass = {
    .ambient = glm::vec3(0.329412f, 0.223529f, 0.027451f),
    .diffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f),
    .specular = glm::vec3(0.992157f, 0.941176f, 0.807843f),
    .shininess = 0.21794872f * 128.0f,
};

global_variable mat_color_s g_bronze = {
    .ambient = glm::vec3(0.2125f, 0.1275f, 0.054f),
    .diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f),
    .specular = glm::vec3(0.393548f, 0.271906f, 0.166721f),
    .shininess = 0.2f * 128.0f,
};

global_variable mat_color_s g_chrome = {
    .ambient = glm::vec3(0.25f, 0.25f, 0.25f),
    .diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
    .specular = glm::vec3(0.774597f, 0.774597f, 0.774597f),
    .shininess = 0.6f * 128.0f,
};

global_variable mat_color_s g_copper = {
    .ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f),
    .diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f),
    .specular = glm::vec3(0.256777f, 0.137622f, 0.086014f),
    .shininess = 0.1f * 128.0f,
};

global_variable mat_color_s g_gold = {
    .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
    .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f),
    .specular = glm::vec3(0.628281f, 0.555802f, 0.366065f),
    .shininess = 0.4f * 128.0f,
};

global_variable mat_color_s g_silver = {
    .ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f),
    .diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f),
    .specular = glm::vec3(0.508273f, 0.508273f, 0.508273f),
    .shininess = 0.4f * 128.0f,
};

global_variable mat_color_s g_black_plastic = {
    .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
    .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
    .specular = glm::vec3(0.50f, 0.50f, 0.50f),
    .shininess = 0.25f * 128.0f,
};

global_variable mat_color_s g_cyan_plastic = {
    .ambient = glm::vec3(0.0f, 0.1f, 0.06f),
    .diffuse = glm::vec3(0.0f, 0.50980392f, 0.50980392f),
    .specular = glm::vec3(0.50196078f, 0.50196078f, 0.50196078f),
    .shininess = 0.25f * 128.0f,
};

global_variable mat_color_s g_green_plastic = {
    .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
    .diffuse = glm::vec3(0.1f, 0.35f, 0.1f),
    .specular = glm::vec3(0.45f, 0.55f, 0.45f),
    .shininess = 0.25f * 128.0f,
};

global_variable mat_color_s g_red_plastic = {
    .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
    .diffuse = glm::vec3(0.5f, 0.0f, 0.0f),
    .specular = glm::vec3(0.7f, 0.6f, 0.6f),
    .shininess = 0.25f * 128.0f,
};

global_variable mat_color_s g_white_plastic = {
    .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
    .diffuse = glm::vec3(0.55f, 0.55f, 0.55f),
    .specular = glm::vec3(0.70f, 0.70f, 0.70f),
    .shininess = 0.25f * 128.0f,
};
global_variable mat_color_s g_yellow_plastic = {
    .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
    .diffuse = glm::vec3(0.5f, 0.5f, 0.0f),
    .specular = glm::vec3(0.60f, 0.60f, 0.50f),
    .shininess = 0.25f * 128.0f,
};
global_variable mat_color_s g_black_rubber = {
    .ambient = glm::vec3(0.02f, 0.02f, 0.02f),
    .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
    .specular = glm::vec3(0.4f, 0.4f, 0.4f),
    .shininess = 0.078125f * 128.0f,
};
global_variable mat_color_s g_cyan_rubber = {
    .ambient = glm::vec3(0.0f, 0.05f, 0.05f),
    .diffuse = glm::vec3(0.4f, 0.5f, 0.5f),
    .specular = glm::vec3(0.04f, 0.7f, 0.7f),
    .shininess = 0.078125f * 128.0f,
};
global_variable mat_color_s g_green_rubber = {
    .ambient = glm::vec3(0.0f, 0.05f, 0.0f),
    .diffuse = glm::vec3(0.4f, 0.5f, 0.4f),
    .specular = glm::vec3(0.04f, 0.7f, 0.04f),
    .shininess = 0.078125f * 128.0f,
};
global_variable mat_color_s g_red_rubber = {
    .ambient = glm::vec3(0.05f, 0.0f, 0.0f),
    .diffuse = glm::vec3(0.5f, 0.4f, 0.4f),
    .specular = glm::vec3(0.7f, 0.04f, 0.04f),
    .shininess = 0.078125f * 128.0f,
};

global_variable mat_color_s g_white_rubber = {
    .ambient = glm::vec3(0.05f, 0.05f, 0.05f),
    .diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
    .specular = glm::vec3(0.7f, 0.7f, 0.7f),
    .shininess = 0.078125f * 128.0f,
};

global_variable mat_color_s g_yellow_rubber = {
    .ambient = glm::vec3(0.05f, 0.05f, 0.0f),
    .diffuse = glm::vec3(0.5f, 0.5f, 0.4f),
    .specular = glm::vec3(0.7f, 0.7f, 0.04f),
    .shininess = 0.078125f * 128.0f,
};

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