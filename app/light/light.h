#ifndef LIGHT_H
#define LIGHT_H

#include "unity.h"

#include "debug.h"
#include "flycamera.h"
#include "mat_color.cpp"
#include "mat_tex.cpp"
// #include "mesh_renderer.h"
#include "game_object.cpp"
#include "game_object.h"
#include "memory/base_memory.h"
#include "memory/base_memory_malloc.cpp"
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

const int g_maze_size = 10;
const int g_maze_objects = g_maze_size * g_maze_size * 3;
const int GOSize = 4 + 1 + g_maze_objects + 1 + 1;
const int LampInstance = 12;
const int BoxInstance = 13;
const int MazeInstance = 14;
const int Ramp1Instance = 15;
const int Ramp2Instance = 16;

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
  MArena arena = {};

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
                                        glm::mat4 model, Camera *camera);

internal void app_update_dirlight(light_s *l, glm::mat4 view);
internal void update_color_rainbow(light_s *l);
internal void update_move_zigzag(glm::vec3 *pos);
internal void draw_maze(Scene *app, Camera *camera);
internal void draw_ramp1(Scene *app, Camera *camera);
internal void draw_ramp2(Scene *app, Camera *camera);

internal void sceneLampDraw(Scene *scene, GameObject *lamp);
internal void draw_material_preview(Scene *app, Camera *camera);
internal void sceneRenderMatColor(Scene *scn, GameObject *obj);

internal int sceneMazeStart(GameObject *objectArena, mesh_s *mesh,
                            shader_s *shader, light_s *lightSource);

internal light_s *sceneDefaultLight(Scene *scn);

int rnd = 41241515;
int rnd_mod = 489414;
int rnd_step = 12345;

internal light_s sceneMissingLight = {
    .position = vec3(0, 0, 0),
    .direction = vec3(1, 0, 0),
    .ambient = vec3(247 / 255 / 3, 161 / 255 / 3, 188 / 255 / 3),
    .diffuse = vec3(247 / 255 / 2, 161 / 255 / 2, 188 / 255 / 2),
    .specular = vec3(247 / 255 / 1, 161 / 255 / 1, 188 / 255 / 1),
    .constant = 1.0f,
    .linear = 0.9f,
    .quadratic = 0.032f,

    .cutOff = glm::cos(glm::radians(12.5f)),
    .outerCutOff = glm::cos(glm::radians(15.5f))};
#endif