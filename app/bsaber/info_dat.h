#ifndef BSABER_INFO_DAT_H
#define BSABER_INFO_DAT_H

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
  // difficultyBeatmapSets
} infoDat;

typedef struct beatmapSet {
  str8 beatmapCharacteristicName;
  difficultyBeatmaps;
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

#endif