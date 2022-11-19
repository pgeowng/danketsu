#ifndef AUDIO_H
#define AUDIO_H

#include "unity.h"

struct app_s {};

internal bool app_init(app_s* app);
internal void app_update(app_s* app, float delta);
internal void sdl_audio_callback(void* userdata, Uint8* stream, int len);
internal void app_input(app_s* app, SDL_Event e);

#endif