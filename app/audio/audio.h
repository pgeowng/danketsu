#ifndef AUDIO_H
#define AUDIO_H

#include "unity.h"

struct Scene {};

internal bool app_init(Scene *app);
internal void app_update(Scene *app, float delta);
internal void sdl_audio_callback(void *userdata, Uint8 *stream, int len);
internal void app_input(Scene *app, SDL_Event e);

#endif