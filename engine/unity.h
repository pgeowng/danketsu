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
#include <SDL.h>
#include <SDL_opengl.h>
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
using glm::vec2;
using glm::vec3;
using glm::vec4;

#include "base_types.h"

// TODO: temp solution for not overwritting sprint function. Because there is a
//       warning C4996. And _s function handles some parameters errors.
//       The proper way would be my own implementation of string function
//       wrappers.
#define mysprintf sprintf_s
#define myfscanf fscanf_s
#define myfopen fopen_s

static int nuReadString(FILE *stream, const char *format, char *buffer,
                        int bufferSize) {
  return fscanf_s(stream, format, buffer, bufferSize);
}

static FILE *nuOpenFile(const char *filename, const char *mode) {
  FILE *f = NULL;
  fopen_s(&f, filename, mode);
  return f;
}

static char *nuStrcpy(char *dest, int destCap, const char *src) {
  strcpy_s(dest, destCap, src);
  return dest;
}

#endif