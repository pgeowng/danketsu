#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct light_s {
  // be aware that is position is in view space
  glm::vec3 position;
  // glm::vec3 color;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
} g_light = {
  glm::vec3(0.0f, 0.0f, 0.0f), // position
  // glm::vec3(1.0f, 1.0f, 1.0f), // color
  glm::vec3(0.2f, 0.2f, 0.2f), // ambient
  glm::vec3(0.5f, 0.5f, 0.5f), // diffuse
  glm::vec3(1.0f, 1.0f, 1.0f)  // specular
};

struct mesh_renderer_s {
  // GLuint ebo;
  // GLuint texture;
  // GLuint normal_map;
  // GLuint specular_map;

  shader_s* shader;

  GLuint vao;
  GLuint vbo;
  int num_triangles;
} g_ramp, g_cube;

void mr_set_light(mesh_renderer_s* mr, light_s* light) {
  shader_use(mr->shader);
  shader_3f(mr->shader, "light.position", light->position.x, light->position.y,
            light->position.z);
  shader_3f(mr->shader, "light.ambient", light->ambient.r, light->ambient.g,
            light->ambient.b);
  shader_3f(mr->shader, "light.diffuse", light->diffuse.r, light->diffuse.g,
            light->diffuse.b);
  shader_3f(mr->shader, "light.specular", light->specular.r, light->specular.g,
            light->specular.b);
}

void mr_set_projection(mesh_renderer_s* mr, glm::mat4 model, glm::mat4 view,
                       glm::mat4 proj, glm::vec3 camera_pos_view) {
  shader_use(mr->shader);
  shader_3f(mr->shader, "viewPos", camera_pos_view.x, camera_pos_view.y,
            camera_pos_view.z);

  shader_mat4fv(mr->shader, "model", glm::value_ptr(model));
  shader_mat4fv(mr->shader, "view", glm::value_ptr(view));
  shader_mat4fv(mr->shader, "projection", glm::value_ptr(proj));
}

void mr_render(mesh_renderer_s* mr) {
  glBindVertexArray(mr->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mr->vbo);
  glDrawArrays(GL_TRIANGLES, 0, mr->num_triangles);
}

#endif