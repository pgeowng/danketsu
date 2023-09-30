#include "light.h"

internal void app_input(Scene *app, SDL_Event e) {
  static int input_move_forward = 0;
  static int input_move_right = 0;
  static int input_move_left = 0;
  static int input_move_back = 0;
  static SDL_bool g_relative_mouse_mode = SDL_FALSE;
  switch (e.type) {
  case SDL_WINDOWEVENT: {
    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
      int g_screenWidth = e.window.data1;
      int g_screenHeight = e.window.data2;
      glViewport(0, 0, g_screenWidth, g_screenHeight);
      // is it good to do this here?
      app->camera.aspect = (float)g_screenWidth / (float)g_screenHeight;
    }
    break;
  }
  case SDL_KEYDOWN:
  case SDL_KEYUP: {
    float forward = 0;
    float right = 0;
    int pressed = e.key.state == SDL_PRESSED;
    switch (e.key.keysym.sym) {
    case SDLK_w: {
      input_move_forward = pressed;
      break;
    }
    case SDLK_a: {
      input_move_left = pressed;
      break;
    };
    case SDLK_s: {
      input_move_back = pressed;
      break;
    }
    case SDLK_d: {
      input_move_right = pressed;
      break;
    }
    case SDLK_e: {
      if (!pressed) {
        app->enable_maze = !app->enable_maze;
      }
      break;
    }
    case SDLK_q: {
      if (!pressed) {
        app->enable_mat_color = !app->enable_mat_color;
      }
      break;
    }
    case SDLK_ESCAPE: {
      if (!pressed) {
        g_relative_mouse_mode =
            (g_relative_mouse_mode == SDL_TRUE) ? SDL_FALSE : SDL_TRUE;
        int ok = SDL_SetRelativeMouseMode(g_relative_mouse_mode);
        if (ok) {
          printf("sdl set relative mouse mode failed: %s\n", SDL_GetError());
        }
      }
      break;
    }
    case SDLK_LSHIFT: {
      app->camera.speed = pressed ? 10.0f : 5.0f;
      break;
    }
    }

    flycamera_process_movement(&app->camera,
                               input_move_forward - input_move_back,
                               input_move_right - input_move_left);
    break;
  }

  case SDL_MOUSEMOTION: {
    flycamera_process_mouse_movement(&app->camera, (float)(e.motion.xrel),
                                     (float)(e.motion.yrel));
    break;
  }

  case SDL_MOUSEWHEEL: {
    flycamera_process_mouse_scroll(&app->camera, (float)(e.wheel.y));
    break;
  }
  }
}