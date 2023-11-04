#include "bsaber.h"

static bool appInit(app *p) {

  MArena a = MArenaMake(MMallocBaseMemory());
  // jsmn_parser p;
  // jsmntok_t t[128];
  // jsmn_init(&p);

  // r = jsmn_parse(&p, s, strlen(s), t, 128);

  str8 readFileNameContent;
  i32 err = emReadFile(&a, str8Lit("./assets/bsaber/read_file_name.txt"),
                       &readFileNameContent);

  if (err) {
    LogError("ReadFileNameContent failed");
    return 0;
  }

  LogInfo((const char *)readFileNameContent.str);

  str8 infoDatContent;
  err = emReadFile(&a, readFileNameContent, &infoDatContent);
  if (err) {
    LogError("ReadInfoDat failed");
    return 0;
  }

  return 1;
}
static void appUpdate(app *p, f32 delta) {}
static void appInput(app *p, SDL_Event e) {}
static void appClean(app *p) {}