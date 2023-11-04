#include "info_dat.h"

#define JSON_PARSE_STR 8085
#define JSON_PARSE_F32 5578
#define JSON_SKIP_CHILDREN 4245
#define JSON_EXPECT_ROOT_KEY 0
#define JSON_EXPECT_ROOT_OBJECT 124

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

      if (str8Equal(str8Lit("_difficultyBeatmapSets", key))) {

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

static b8 beatmapSetUnmarshal()