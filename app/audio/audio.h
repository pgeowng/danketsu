#ifndef AUDIO_H
#define AUDIO_H

#include "unity.h"

struct App {};

internal bool app_init(App *app);
internal void app_update(App *app, float delta);
internal void sdl_audio_callback(void *userdata, Uint8 *stream, int len);
internal void app_input(App *app, SDL_Event e);

#endif