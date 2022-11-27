#ifndef LIGHT_SHADER_H
#define LIGHT_SHADER_H

#include "unity.h"

#include "shader.h"

struct light_s {
  // coords and directions must be in view space
  vec3 position; // not used by directional light
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // used by pointlight
  float constant = 1.0f;
  float linear = 0.9f;
  float quadratic = 0.032f;

  // used by spotlight
  float cutOff = glm::cos(glm::radians(12.5f));
  float outerCutOff = glm::cos(glm::radians(15.5f));

};

void shader_set_light(shader_s* sh, light_s* light) {
  shader_use(sh);

  shader_3f(sh, "light.ambient", light->ambient.r, light->ambient.g,
            light->ambient.b);
  shader_3f(sh, "light.diffuse", light->diffuse.r, light->diffuse.g,
            light->diffuse.b);
  shader_3f(sh, "light.specular", light->specular.r, light->specular.g,
            light->specular.b);

  // dirlight
  shader_3f(sh, "light.direction", light->direction.x,
            light->direction.y, light->direction.z);

  // pointlight
  shader_3f(sh, "light.position", light->position.x, light->position.y,
            light->position.z);
}

void shader_set_dirlight(shader_s* sh, light_s* l) {
  shader_use(sh);
  shader_3f(sh, "dirLight.ambient", l->ambient.r, l->ambient.g, l->ambient.b);
  shader_3f(sh, "dirLight.diffuse", l->diffuse.r, l->diffuse.g, l->diffuse.b);
  shader_3f(sh, "dirLight.specular", l->specular.r, l->specular.g,
            l->specular.b);

  shader_3f(sh, "dirLight.direction", l->direction.x, l->direction.y,
            l->direction.z);
}

void shader_set_pointlight(shader_s* sh, mat4 view, light_s* l, int l_idx) {
  char key[50];
  shader_use(sh);

  sprintf(key, "pointLights[%d].%s", l_idx, "position");
  vec3 position = vec3(view * vec4(l->position, 1.0f));
  shader_3f(sh, key, position.x, position.y, position.z);

  sprintf(key, "pointLights[%d].%s", l_idx, "ambient");
  shader_3f(sh, key, l->ambient.x, l->ambient.y, l->ambient.z);

  sprintf(key, "pointLights[%d].%s", l_idx, "diffuse");
  shader_3f(sh, key, l->diffuse.x, l->diffuse.y, l->diffuse.z);

  sprintf(key, "pointLights[%d].%s", l_idx, "specular");
  shader_3f(sh, key, l->specular.x, l->specular.y, l->specular.z);

  sprintf(key, "pointLights[%d].%s", l_idx, "constant");
  shader_1f(sh, key, l->constant);

  sprintf(key, "pointLights[%d].%s", l_idx, "linear");
  shader_1f(sh, key, l->linear);

  sprintf(key, "pointLights[%d].%s", l_idx, "quadratic");
  shader_1f(sh, key, l->quadratic);
}

void shader_set_spotlight(shader_s* sh, light_s* l) {
  shader_use(sh);
  shader_3f(sh, "spotLight.position", l->position.x, l->position.y,
            l->position.z);
  shader_3f(sh, "spotLight.direction", l->direction.x, l->direction.y,
            l->direction.z);
  shader_3f(sh, "spotLight.ambient", l->ambient.x, l->ambient.y, l->ambient.z);
  shader_3f(sh, "spotLight.diffuse", l->diffuse.x, l->diffuse.y, l->diffuse.z);
  shader_3f(sh, "spotLight.specular", l->specular.x, l->specular.y,
            l->specular.z);
  shader_1f(sh, "spotLight.cutOff", l->cutOff);
  shader_1f(sh, "spotLight.outerCutOff", l->outerCutOff);
}

void shader_set_lightsrc(shader_s* sh, glm::vec3 col) {
  shader_use(sh);
  shader_3f(sh, "lightColor", col.x, col.y, col.z);
}

void shader_set_transform(shader_s* sh, glm::mat4 model, glm::mat4 view,
                          glm::mat4 proj) {
  shader_mat4fv(sh, "model", glm::value_ptr(model));
  shader_mat4fv(sh, "view", glm::value_ptr(view));
  shader_mat4fv(sh, "projection", glm::value_ptr(proj));
}

void shader_set_transform_and_viewpos(shader_s* sh, mat4 model, mat4 view,
                       mat4 proj, vec3 camera_pos_view) {
  shader_use(sh);
  shader_3f(sh, "viewPos", camera_pos_view.x, camera_pos_view.y,
            camera_pos_view.z);

  shader_mat4fv(sh, "model", glm::value_ptr(model));
  shader_mat4fv(sh, "view", glm::value_ptr(view));
  shader_mat4fv(sh, "projection", glm::value_ptr(proj));
}
#endif