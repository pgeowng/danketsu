#ifndef LIGHT_H
#define LIGHT_H

#include "unity.h"

#include "debug.h"
#include "flycamera.h"
#include "mat_color.cpp"
#include "mat_tex.cpp"
#include "mesh_renderer.h"
#include "shader.h"
#include "texture.h"

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

  mat_color_s mat_color;
  mat_tex_s mat_tex = { 0 };

  flycamera_s camera = {};
  shader_s lighting_shader = {};
  shader_s lamp_shader = {};
};

#define internal static

internal bool app_init(app_s* app);
internal bool app_init_tex(app_s* app, GLuint* tex_unit, const char* path,
                           GLenum tex_unit_enum);

internal void app_render_mat_color_cube(app_s* app, mesh_renderer_s* cube,
                                        glm::mat4 model, glm::mat4 view,
                                        glm::mat4 proj,
                                        glm::vec3 camera_view_pos,
                                        light_s* light);
#endif