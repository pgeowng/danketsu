#ifndef BSABER_INFO_DAT_H
#define BSABER_INFO_DAT_H

#include "emslice.h"

typedef struct infoDat {
  str8 version;
  str8 songName;
  str8 songSubName;
  str8 songAuthorName;
  str8 levelAuthorName;
  f32 beatsPerMinute;
  f32 shuffle;
  f32 shufflePeriod;
  f32 previewStartTime;
  f32 previewDuration;
  str8 songFilename;
  str8 coverImageFilename;
  str8 environmentName;
  str8 allDirectionsEnvironmentName;
  f32 songTimeOffset;
  // environmentName
  // colorSchemes
  // customData
  emArr difficultyBeatmapSets;
} infoDat;

typedef struct beatmapSet {
  str8 beatmapCharacteristicName;
  emArr difficultyBeatmaps;
} beatmapSet;

typedef struct beatmap {
  str8 difficulty;
  f32 difficultyRank;
  str8 beatmapFilename;
  f32 noteJumpMovementSpeed;
  f32 noteJumpStartBeatOffset;
  u32 beatmapColorSchemeIdx;
  u32 environmentNameIdx;
  // customData

} beatmap;

static i32 beatmapSetUnmarshal(str8 infoDatContent, jsmntok_t *jsonTokens,
                               u64 size, emArr *beatmapSets);
static i32 beatmapUnmarshal(str8 infoDatContent, jsmntok_t *jsonTokens,
                            u64 size, emArr *beatmaps);

#endif