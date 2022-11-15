#ifndef UNITY_H
#define UNITY_H

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include <GL/glew.h>

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
#include <cmath>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define internal static

#endif