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

struct material_s {
  glm::vec3 ambient, diffuse, specular;
  float shininess;
} g_mat0 = {
  .ambient = glm::vec3(1.0f, 0.5f, 0.31f),
  .diffuse = glm::vec3(1.0f, 0.5f, 0.31f),
  .specular = glm::vec3(0.5f, 0.5f, 0.5f),
  .shininess = 32.0f,
}, g_mat1 = {
    .ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f),
    .diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f),
    .specular = glm::vec3(0.256777f, 0.137622f, 0.086014f),
    .shininess = 12.8f,
}, g_mat2 = {
    .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
    .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f),
    .specular = glm::vec3(0.0f, 0.0f, 0.0f),
    .shininess = 0.1f,
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

void mr_render(mesh_renderer_s* mr, material_s mat, glm::mat4 model,
               glm::mat4 view, glm::mat4 proj, glm::vec3 camera_pos_view,
               light_s* light) {
  shader_use(mr->shader);

  shader_3f(mr->shader, "viewPos", camera_pos_view.x, camera_pos_view.y,
            camera_pos_view.z);

  shader_3f(mr->shader, "light.position", light->position.x, light->position.y,
            light->position.z);
  shader_3f(mr->shader, "light.ambient", light->ambient.r, light->ambient.g,
            light->ambient.b);
  shader_3f(mr->shader, "light.diffuse", light->diffuse.r, light->diffuse.g,
            light->diffuse.b);
  shader_3f(mr->shader, "light.specular", light->specular.r, light->specular.g,
            light->specular.b);

  shader_3f(mr->shader, "material.ambient", mat.ambient.r, mat.ambient.g,
            mat.ambient.b);
  shader_3f(mr->shader, "material.diffuse", mat.diffuse.r, mat.diffuse.g,
            mat.diffuse.b);
  shader_3f(mr->shader, "material.specular", mat.specular.r, mat.specular.g,
            mat.specular.b);
  // shader_Set3f(mr->shader, "material.emission", 0.0f, 0.0f, 0.0f);
  shader_1f(mr->shader, "material.shininess", mat.shininess);

  shader_mat4fv(mr->shader, "model", glm::value_ptr(model));
  shader_mat4fv(mr->shader, "view", glm::value_ptr(view));
  shader_mat4fv(mr->shader, "projection", glm::value_ptr(proj));

  glBindVertexArray(mr->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mr->vbo);
  glDrawArrays(GL_TRIANGLES, 0, mr->num_triangles);
}

#endif