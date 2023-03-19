#ifndef LIGHT_H
#define LIGHT_H

#include "unity.h"

#include "debug.h"
#include "flycamera.h"
#include "mat_color.cpp"
#include "mat_tex.cpp"
// #include "mesh_renderer.h"
#include "game_object.h"
#include "mesh.cpp"
#include "mesh.h"
#include "raycast.h"
#include "shader.h"
#include "text.h"
#include "texture.h"

#include "example/cube_mesh.h"
#include "example/cube_tex_mesh.h"
#include "example/ramp_mesh.h"
#include "light_shader.h"


const int GOSize = 4;
const int LampInstance = 12;

struct Scene {
  // glm::vec3 position;
  // glm::vec3 color;
  // glm::vec3 ambient;
  // glm::vec3 diffuse;
  // glm::vec3 specular;

  // GLuint vao;
  // GLuint light_vao;
  // GLuint vbo;
  // GLuint ramp_vao;
  // GLuint ramp_vbo;
  // GLuint ebo;

  mesh_s ramp_mesh = {};
  mesh_s texture_cube_mesh = {};
  mesh_s debug_sphere = {};

  bool enable_maze = false;
  bool enable_mat_color = false;

  mat_color_s mat_color;
  mat_tex_s mat_tex = {0};

  Camera camera = {};
  shader_s lighting_shader = {};
  shader_s lamp_shader = {};

  light_s dir_light = {};
  light_s p_light[4] = {};
  light_s sp_light = {};

  text_s text_renderer = {};

  GameObject go[GOSize];
};

#define internal static

internal bool app_init(Scene *app);
internal bool app_init_tex(Scene *app, GLuint *tex_unit, const char *path,
                           GLenum tex_unit_enum);

internal void app_render_mat_color_cube(Scene *app, mesh_s *cube, shader_s *sh,
                                        glm::mat4 model, Camera *camera,
                                        light_s *light);

internal void app_update_dirlight(light_s *l, glm::mat4 view);
internal void update_color_rainbow(light_s *l);
internal void update_move_zigzag(glm::vec3 *pos);
internal void draw_cube(Scene *app, Camera *camera);
internal void draw_maze(Scene *app, Camera *camera);
internal void draw_ramp1(Scene *app, Camera *camera);
internal void draw_ramp2(Scene *app, Camera *camera);

internal void SceneLampDraw(Scene *scene);
internal void draw_material_preview(Scene *app, Camera *camera);

#define MAZE_SIZE 10

int g_maze[MAZE_SIZE * MAZE_SIZE * 3] = {0};
int g_maze_size = MAZE_SIZE;

int rnd = 41241515;
int rnd_mod = 489414;
int rnd_step = 12345;
#endif