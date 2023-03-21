#include "game_object.h"

internal void GameObjectConstruct(GameObject *obj, uint16_t instance,
                                  mat4 transform, mesh_s *mesh,
                                  shader_s *shader, light_s *light,
                                  mat_color_s *mat_color) {
  obj->instance = instance;
  obj->transform = transform;
  obj->mesh = mesh;
  obj->shader = shader;
  obj->light = light;
  obj->mat_color = mat_color;
}