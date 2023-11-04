#include "bsaber.h"

static bool appInit(app *p) {

  MArena a = MArenaMake(MMallocBaseMemory());

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

  infoDat first;

  err = infoDatUnmarshal(&first, infoDatContent);
  if (err) {
    LogError("infoDatUnmarshal failed");
    return 0;
  }

  LogError("_version: %s <-", str8AC(&a, first.version));

  return 1;
}
static void appUpdate(app *p, f32 delta) {}
static void appInput(app *p, SDL_Event e) {}
static void appClean(app *p) {}