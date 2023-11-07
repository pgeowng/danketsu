static b8 loadStorage(MArena *a) {

  str8 readFileNameContent;
  i32 err = emReadFile(a, str8Lit("./assets/bsaber/read_file_name.txt"),
                       &readFileNameContent);

  if (err) {
    LogError("ReadFileNameContent failed");
    return 1;
  }

  LogInfo((const char *)readFileNameContent.str);

  str8 infoDatContent;
  err = emReadFile(a, readFileNameContent, &infoDatContent);
  if (err) {
    LogError("ReadInfoDat failed");
    return 1;
  }

  infoDat first;

  err = infoDatUnmarshal(&first, infoDatContent);
  if (err) {
    LogError("infoDatUnmarshal failed");
    return 1;
  }

  LogError("version: %s", str8AC(a, first.version));
  LogError("songName: %s", str8AC(a, first.songName));
  LogError("songSubName: %s", str8AC(a, first.songSubName));
  LogError("songAuthorName: %s", str8AC(a, first.songAuthorName));
  LogError("levelAuthorName: %s", str8AC(a, first.levelAuthorName));
  LogError("beatsPerMinute: %f", first.beatsPerMinute);
  LogError("shuffle: %f", first.shuffle);
  LogError("shufflePeriod: %f", first.shufflePeriod);
  LogError("previewStartTime: %f", first.previewStartTime);
  LogError("previewDuration: %f", first.previewDuration);
  LogError("songFilename: %s", str8AC(a, first.songFilename));
  LogError("coverImageFilename: %s", str8AC(a, first.coverImageFilename));
  LogError("environmentName: %s", str8AC(a, first.environmentName));
  LogError("allDirectionsEnvironmentName: %s",
           str8AC(a, first.allDirectionsEnvironmentName));
  LogError("songTimeOffset: %f", first.songTimeOffset);

  LogError("difficultyBeatmapSets: len=%d", first.difficultyBeatmapSets.len);
  for (i32 i = 0; i < first.difficultyBeatmapSets.len; i++) {
    beatmapSet *set = emArrGet(&first.difficultyBeatmapSets, beatmapSet, i);
    LogError("  beatmapCharacteristicName: %s",
             str8AC(a, set->beatmapCharacteristicName));

    emArr *bm = &set->difficultyBeatmaps;
    LogError("  difficultyBeatmaps: len=%d", bm->len);
    for (i32 j = 0; j < bm->len; j++) {
      beatmap *b = emArrGet(bm, beatmap, j);
      LogError("    difficulty: %s", str8AC(a, b->difficulty));
      LogError("    difficultyRank: %f", b->difficultyRank);
      LogError("    beatmapFilename: %s", str8AC(a, b->beatmapFilename));
      LogError("    noteJumpMovementSpeed: %f", b->noteJumpMovementSpeed);
      LogError("    noteJumpStartBeatOffset: %f", b->noteJumpStartBeatOffset);
      LogError("    beatmapColorSchemeIdx: %ud", b->beatmapColorSchemeIdx);
      LogError("    environmentNameIdx: %ud", b->environmentNameIdx);
    }
  }
  LogError("version: %s", str8AC(a, first.version));
  LogError("version: %s", str8AC(a, first.version));
  LogError("version: %s", str8AC(a, first.version));

  return 0;
}