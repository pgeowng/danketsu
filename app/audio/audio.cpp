#include "audio.h"

internal bool app_init(app_s* app) {
  bool ok = true;

  SDL_AudioSpec want, have;
  SDL_AudioDeviceID dev;

  SDL_zero(want);
  want.freq = 48000;
  want.format = AUDIO_F32;
  want.channels = 2;
  want.samples = 4096;
  want.callback = sdl_audio_callback;

  dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
  if (dev = 0) {
    printf("failed to open audio: %s\n", SDL_GetError());
    return false;
  }

  if (have.format != want.format) {
    printf("We didn't get Float32 audio format.\n");
    return false;
  }

  SDL_PauseAudioDevice(dev, 0);
  SDL_Delay(5000);
  SDL_CloseAudioDevice(dev);

  return ok;
}

internal void app_update(app_s* app, float delta) {}

internal void sdl_audio_callback(void* userdata, Uint8* stream, int len) {
  static float time = 0.0f;

  const int num_samples = (len / sizeof(float));
  float* output_stream = (float*)stream;

  int i = 0;
  while (i < num_samples) {
    output_stream[i++] = sin(time);
    output_stream[i++] = sin(time);
    time += 0.1f;
  }
}

internal void app_input(app_s* app, SDL_Event e) {}