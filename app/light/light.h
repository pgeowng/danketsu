#ifndef LIGHT_H
#define LIGHT_H

#include "unity.h"

#include "debug.h"
#include "flycamera.h"
#include "shader.h"
#include "texture.h"

#include "mesh_renderer.h"

struct app_s {
  // glm::vec3 position;
  // glm::vec3 color;
  // glm::vec3 ambient;
  // glm::vec3 diffuse;
  // glm::vec3 specular;

  GLuint vao;
  GLuint light_vao;
  GLuint vbo;
  GLuint ramp_vao;
  GLuint ramp_vbo;
  GLuint ebo;

  GLuint texDiffuse;

  flycamera_s camera = {};
  shader_s lighting_shader = {};
  shader_s lamp_shader = {};
};

#define internal static

internal bool app_init(app_s* app);
internal bool app_init_tex(app_s* app, GLuint* tex_unit, const char* path,
                           GLenum tex_unit_enum);

#endif