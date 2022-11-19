#ifndef UNITY_H
#define UNITY_H

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include <GL/glew.h>

#ifdef UNITY_WIN
#include "imgui.cpp"

#include "imgui_demo.cpp"
#include "imgui_draw.cpp"
#include "imgui_tables.cpp"
#include "imgui_widgets.cpp"

#include "imgui_impl_opengl3.cpp"
#include "imgui_impl_sdl.cpp"

#include <GL/glu.h>
#include <SDL_opengl.h>

#include <SDL.h>

#endif

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cmath>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>

#define internal static
#define local_persist static
#define global_variable static
#define COUNT_OF(x)                                                            \
  ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

using glm::mat4;
using glm::scale;
using glm::translate;
using glm::vec3;
using glm::vec4;
#endif