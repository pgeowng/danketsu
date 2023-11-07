#include "info_dat.h"

#define JSON_PARSE_STR 8085
#define JSON_PARSE_F32 5578
#define JSON_SKIP_CHILDREN 4245
#define JSON_EXPECT_ROOT_KEY 0
#define JSON_EXPECT_ROOT_OBJECT 124
#define JSON_EXPECT_ROOT_ARRAY 4125
#define JSON_PARSE_BEATMAP 5515
#define JSON_PARSE_U32 8941

static b8 infoDatUnmarshal(infoDat *info, str8 infoDatContent) {
  jsmn_parser jp;
  jsmntok_t t[128];
  jsmn_init(&jp);

  i32 r = jsmn_parse(&jp, str8C(infoDatContent), infoDatContent.size, t, 128);
  LogInfo("jsmn_parse: count %d", r);

  i32 rootChildCount = 0;
  i32 skipRemain = 0;
  u32 assignOffset = 0;
  i32 state = JSON_EXPECT_ROOT_OBJECT;

  for (i32 i = 0; i < r; i++) {
    jsmntok_t tok = t[i];
    str8 lit = str8New(&infoDatContent.str[tok.start], tok.end - tok.start, 0);

    switch (state) {
    case JSON_EXPECT_ROOT_OBJECT: {
      // expect object at first
      if (tok.type != JSMN_OBJECT) {
        LogError("infoDatUnmarshal: expected object at start of file");
        goto error;
      }
      rootChildCount = tok.size;
      state = JSON_EXPECT_ROOT_KEY;
      break;
    }
    case JSON_EXPECT_ROOT_KEY: {

      // expect key of root object

      // then we are expecting keys of objects.
      if (tok.type != JSMN_STRING && tok.size == 0) {
        LogError("infoDatUnmarshal: expected root object keys");
        goto error;
      }

      str8 key = lit;

      // string fields

      if (str8Equal(str8Lit("_version"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, version);
        continue;
      }

      if (str8Equal(str8Lit("_songName"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, songName);
        continue;
      }

      if (str8Equal(str8Lit("_songSubName"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, songSubName);
        continue;
      }

      if (str8Equal(str8Lit("_songAuthorName"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, songAuthorName);
        continue;
      }

      if (str8Equal(str8Lit("_levelAuthorName"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, levelAuthorName);
        continue;
      }

      if (str8Equal(str8Lit("_songFilename"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, songFilename);
        continue;
      }

      if (str8Equal(str8Lit("_coverImageFilename"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, coverImageFilename);
        continue;
      }

      if (str8Equal(str8Lit("_environmentName"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, environmentName);
        continue;
      }

      if (str8Equal(str8Lit("_allDirectionsEnvironmentName"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(infoDat, allDirectionsEnvironmentName);
        continue;
      }

      if (str8Equal(str8Lit("_beatsPerMinute"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(infoDat, beatsPerMinute);
        continue;
      }

      if (str8Equal(str8Lit("_shuffle"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(infoDat, shuffle);
        continue;
      }

      if (str8Equal(str8Lit("_shufflePeriod"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(infoDat, shufflePeriod);
        continue;
      }

      if (str8Equal(str8Lit("_previewStartTime"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(infoDat, previewStartTime);
        continue;
      }

      if (str8Equal(str8Lit("_previewDuration"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(infoDat, previewDuration);
        continue;
      }

      if (str8Equal(str8Lit("_songTimeOffset"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(infoDat, songTimeOffset);
        continue;
      }

      if (str8Equal(str8Lit("_difficultyBeatmapSets"), key)) {
        i32 offset = beatmapSetUnmarshal(infoDatContent, &t[i + 1], r - i - 1,
                                         &info->difficultyBeatmapSets);
        if (offset == 0) {
          LogError("infoDatUnmarshal: expected object");
          goto error;
        }
        i = i + offset - 1;

        continue;
      }

      if (tok.size > 0) {
        state = JSON_SKIP_CHILDREN; // skip children
        skipRemain += tok.size;
      } else {
        state = JSON_EXPECT_ROOT_KEY; // expect key
      }

      // if (str8Equal(str8Lit("")))
      // str8 keys[...] = {
      //   ,
      //   str8Lit("_songName"),
      //   str8Lit("_songSubName"),
      //   str8Lit("_songAuthorName"),
      //   str8Lit("_levelAuthorName"),
      //   str8Lit("_beatsPerMinute"),
      //   str8Lit("_shuffle"),
      //   str8Lit("_shufflePeriod"),
      //   str8Lit("_previewStartTime"),
      //   str8Lit("_previewDuration"),
      //   str8Lit("_songFilename"),
      //   str8Lit("_coverImageFilename"),
      //   str8Lit("_environmentName"),
      //   str8Lit("_allDirectionsEnvironmentName"),
      //   str8Lit("_songTimeOffset"),
      //   str8Lit("_customData")
      // } if (str8str8Lit("_version"))
      break;
    }
    case JSON_PARSE_STR: {
      // assign string value to field
      if (tok.type != JSMN_STRING && tok.size != 0) {
        LogError("infoDatUnmarshal: expected string value");
        goto error;
      }

      *((str8 *)((u8 *)info + assignOffset)) = lit;

      // go to read next field
      state = JSON_EXPECT_ROOT_KEY;
      break;
    }
    case JSON_PARSE_F32: {
      // todo(em): parse float function
      b8 err = str8ReadF32(lit, (f32 *)((u8 *)info + assignOffset));
      if (err) {
        LogError("infoDatUnmarshal: expected float value");
        goto error;
      }

      // go to read next field
      state = JSON_EXPECT_ROOT_KEY;

      break;
    }
    case JSON_SKIP_CHILDREN: {
      skipRemain--;
      // skip values we are not interested to
      skipRemain += tok.size;

      if (skipRemain == 0) {
        state = JSON_EXPECT_ROOT_KEY;
      }

      break;
    }
    }
  }

  return 0;

error:

  return 1;
}

static i32 beatmapSetUnmarshal(str8 infoDatContent, jsmntok_t *jsonTokens,
                               u64 size, emArr *beatmapSets) {
  i32 state = JSON_EXPECT_ROOT_ARRAY;
  i32 rootSize = 0;
  i32 keysRemain = 0;
  u64 beatmapSetIndex = 0;
  u64 assignOffset = 0;
  u64 skipRemain = 0;
  beatmapSet *currentSet = NULL;
  for (i32 i = 0; i < size; i++) {
    jsmntok_t tok = jsonTokens[i];
    switch (state) {
    case JSON_EXPECT_ROOT_ARRAY: {
      // expect array at first
      if (tok.type != JSMN_ARRAY) {
        LogError("beatmapSetUnmarshal: expected array for key");
        goto error;
      }

      *beatmapSets = emArrNewCap(beatmapSet, tok.size);

      skipRemain += tok.size;

      rootSize = tok.size;
      state = JSON_EXPECT_ROOT_OBJECT;
      break;
    }
    case JSON_EXPECT_ROOT_OBJECT: {
      // expect object values
      if (tok.type != JSMN_OBJECT) {
        LogError("beatmapSetUnmarshal: expected array values to be objects");
        goto error;
      }

      rootSize--;

      currentSet = (beatmapSet *)emArrPush(beatmapSets);

      keysRemain = tok.size;
      state = JSON_EXPECT_ROOT_KEY;
      break;
    }
    case JSON_EXPECT_ROOT_KEY: {
      if (tok.type != JSMN_STRING && tok.size != 1) {
        LogError("beatmapSetUnmarshal: expected key value");
        goto error;
      }

      keysRemain--;

      str8 key =
          str8New(&infoDatContent.str[tok.start], tok.end - tok.start, 0);
      if (str8Equal(str8Lit("_beatmapCharacteristicName"), key)) {
        state = JSON_PARSE_STR,
        assignOffset = offsetof(beatmapSet, beatmapCharacteristicName);
        continue;
      }

      if (str8Equal(str8Lit("_difficultyBeatmaps"), key)) {
        i32 offset =
            beatmapUnmarshal(infoDatContent, &jsonTokens[i + 1], size - i - 1,
                             &currentSet->difficultyBeatmaps);

        if (offset == 0) {
          LogError("infoDatUnmarshal: expected object");
          goto error;
        }

        i = i + offset - 1;
        continue;
      }

      break;
    }

    case JSON_PARSE_STR: {
      // assign string value to field
      if (tok.type != JSMN_STRING && tok.size != 0) {
        LogError("infoDatUnmarshal: expect string value");
        goto error;
      }

      str8 lit =
          str8New(&infoDatContent.str[tok.start], tok.end - tok.start, 0);

      *FieldPtr(currentSet, str8, assignOffset) = lit;

      state = JSON_EXPECT_ROOT_KEY;

      if (keysRemain == 0) {
        state = JSON_EXPECT_ROOT_OBJECT;
      }

      break;
    }
    }
  }

  return (i32)size;

error:

  return 0;
}

static i32 beatmapUnmarshal(str8 infoDatContent, jsmntok_t *jsonTokens,
                            u64 size, emArr *beatmaps) {
  i32 state = JSON_EXPECT_ROOT_ARRAY;
  beatmap *currentSet = NULL;
  u64 arraySize = 0;
  u64 keysRemain = 0;
  u64 assignOffset = 0;
  for (i32 i = 0; i < size; i++) {
    jsmntok_t tok = jsonTokens[i];
    switch (state) {
    case JSON_EXPECT_ROOT_ARRAY: {

      if (tok.type != JSMN_ARRAY) {
        LogError("beatmapUnmarshal: expected array");
        goto error;
      }

      *beatmaps = emArrNewCap(beatmap, tok.size);

      arraySize = tok.size;

      state = JSON_EXPECT_ROOT_OBJECT;

      break;
    }
    case JSON_EXPECT_ROOT_OBJECT: {
      // expect object values
      if (tok.type != JSMN_OBJECT) {
        LogError("beatmapUnmarshal: expected array of objects");
        goto error;
      }

      currentSet = (beatmap *)emArrPush(beatmaps);
      keysRemain = tok.size;
      state = JSON_EXPECT_ROOT_KEY;
      break;
    }

    case JSON_EXPECT_ROOT_KEY: {
      if (tok.type != JSMN_STRING && tok.size != 1) {
        LogError("beatmapUnmarshal: expected object key");
        goto error;
      }

      keysRemain--;

      str8 key =
          str8New(&infoDatContent.str[tok.start], tok.end - tok.start, 0);
      if (str8Equal(str8Lit("_difficulty"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(beatmap, difficulty);
        continue;
      }

      if (str8Equal(str8Lit("_beatmapFilename"), key)) {
        state = JSON_PARSE_STR;
        assignOffset = offsetof(beatmap, beatmapFilename);
        continue;
      }

      if (str8Equal(str8Lit("_difficultyRank"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(beatmap, difficultyRank);
        continue;
      }

      if (str8Equal(str8Lit("_noteJumpMovementSpeed"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(beatmap, noteJumpMovementSpeed);
        continue;
      }

      if (str8Equal(str8Lit("_noteJumpStartBeatOffset"), key)) {
        state = JSON_PARSE_F32;
        assignOffset = offsetof(beatmap, noteJumpStartBeatOffset);
        continue;
      }

      if (str8Equal(str8Lit("_beatmapColorSchemeIdx"), key)) {
        state = JSON_PARSE_U32;
        assignOffset = offsetof(beatmap, beatmapColorSchemeIdx);
        continue;
      }

      if (str8Equal(str8Lit("_environmentNameIdx"), key)) {
        state = JSON_PARSE_U32;
        assignOffset = offsetof(beatmap, environmentNameIdx);
        continue;
      }

      break;
    }
    case JSON_PARSE_STR: {
      if (tok.type != JSMN_STRING && tok.size != 0) {
        LogError("beatmapUnmarshal: expect string value");
        goto error;
      }

      str8 lit =
          str8New(&infoDatContent.str[tok.start], tok.end - tok.start, 0);
      *FieldPtr(currentSet, str8, assignOffset) = lit;

      state = JSON_EXPECT_ROOT_KEY;

      if (keysRemain == 0) {
        state = JSON_EXPECT_ROOT_OBJECT;
      }
      break;
    }

    case JSON_PARSE_F32: {

      str8 lit =
          str8New(&infoDatContent.str[tok.start], tok.end - tok.start, 0);
      b8 err = str8ReadF32(lit, FieldPtr(currentSet, f32, assignOffset));
      if (err) {
        LogError("beatmapUnmarshal: expected float value");
        goto error;
      }

      state = JSON_EXPECT_ROOT_KEY;

      if (keysRemain == 0) {
        state = JSON_EXPECT_ROOT_OBJECT;
      }

      break;
    }
    case JSON_PARSE_U32: {

      str8 lit =
          str8New(&infoDatContent.str[tok.start], tok.end - tok.start, 0);
      b8 err = str8ReadU32(lit, FieldPtr(currentSet, u32, assignOffset));
      if (err) {
        LogError("beatmapUnmarshal: expected u32");
        goto error;
      }

      state = JSON_EXPECT_ROOT_KEY;
      if (keysRemain == 0) {
        state = JSON_EXPECT_ROOT_OBJECT;
      }

      break;
    }
    }
  }

  return (u32)size;

error:

  return 0;
}