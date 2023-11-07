#ifndef EMSTRING_H
#define EMSTRING_H

#include "memory/base_memory.h"
#include "memory/base_memory_malloc.cpp"

typedef struct str8 {
  u8 *str;
  u64 size;
  b8 hasNull;
} str8;

typedef struct str32 {
  u32 *str;
  u64 size;
  b8 hasNull;
} str32;

typedef struct str16 {
  u16 *str;
  u64 size;
  b8 hasNull;
} str16;

internal str8 str8New(u8 *str, u64 size, b8 hasNull) {
  if (hasNull) {
    assert(size != 0);
  }
  str8 s;
  s.str = str;
  s.size = size;
  s.hasNull = hasNull;
  return s;
}

internal str32 str32New(u32 *str, u64 size, b8 hasNull) {
  if (hasNull) {
    assert(size != 0);
  }
  str32 s;
  s.str = str;
  s.size = size;
  s.hasNull = hasNull;
  return s;
}

#define str8Lit(s) str8New((u8 *)(s), sizeof(s) - 1, 1)
#define str32Lit(s) str32New((u32 *)(s), sizeof(s) - 1, 1)
#define str8Expand(s) (int)((s).size), ((s).size)

static str32 str8To32(MArena *a, str8 from) {
  i32 newStringCap = (i32)(from.size + 1);
  u32 *newString = (u32 *)MArenaPush(a, sizeof(u32) * newStringCap);

  u32 ch = 0;
  i32 remain = 0;
  u32 *p = newString;
  i32 newStringSize = 0;
  for (int i = 0; i < from.size; i++) {
    u8 ch1 = from.str[i];
    if (remain > 0) {
      if ((ch1 & 0xC0) != 0x80) {
        LogError(
            "str8To32: malformed utf-8 string: at index %d expected remain: %d",
            remain);
        goto defer;
      }
      ch = ch << 6;
      ch = (ch1 & 0x3F) | ch;
      remain--;
      if (remain == 0) {
        assert(newStringSize + 2 <= newStringCap);
        *p++ = ch;
        newStringSize++;
        ch = 0;
      }

      continue;
    }

    if ((ch1 & 0xF8) == 0xF0) {
      ch = ch1 & 0x07;
      remain = 3;
      continue;
    } else if ((ch1 & 0xF0) == 0xE0) {
      ch = ch1 & 0x0F;
      remain = 2;
      continue;
    } else if ((ch1 & 0xE0) == 0xC0) {
      ch = ch1 & 0x1F;
      remain = 1;
      continue;
    } else if ((ch1 & 0x80) == 0x00) {
      ch = ch1 & 0x7F;

      assert(newStringSize + 2 <= newStringCap);
      *p++ = ch;
      newStringSize++;
      ch = 0;
    }
  }

  if (remain > 0) {
    LogError("str8To32: partial utf-8 string: maybe forgot zero at the end?");
  }

  *p++ = 0;
  // newStringSize++;

defer:
  str32 result;
  result.str = newString;
  result.size = newStringSize;
  result.hasNull = 1;
  return result;
}

static str16 str32To16(MArena *a, str32 from) {
  i32 cap = (i32)(from.size * 2 + 1);
  u16 *newString = (u16 *)MArenaPush(a, sizeof(u16) * cap);

  u16 *p = newString;
  i32 len = 0;
  for (int i = 0; i < from.size; i++) {
    u32 ch = from.str[i];
    if (ch < 0x10000) {
      *p++ = (u16)(ch);
      len++;
    } else {
      ch -= 0x10000;
      *p++ = (u16)((ch >> 10) | 0xD800);   // hi
      *p++ = (u16)((ch & 0x3FF) | 0xDC00); // lo
      len += 2;
    }
  }

  assert(len + 1 <= cap);
  *p = 0;

  str16 result;
  result.str = newString;
  result.size = len;
  result.hasNull = 1;
  return result;
}

static const b8 str8Equal(str8 a, str8 other) {
  if (a.size != other.size) {
    return 0;
  }
  if (a.size == 0) {
    return 1;
  }
  if (a.str[0] != other.str[0]) {
    return 0;
  }
  for (i32 i = 1; i < a.size; i++) {
    if (a.str[i] != other.str[i]) {
      return 0;
    }
  }
  return 1;
}

static str8 str8NullTerminate(MArena *a, str8 s) {
  if (s.hasNull) {
    return s;
  }

  u8 *newString = (u8 *)MArenaPush(a, sizeof(u8) * (s.size + 1));
  u8 *p = newString;
  for (i32 i = 0; i < s.size; i++) {
    *p++ = s.str[i];
  }
  *p = 0;

  str8 str;
  str.str = newString;
  str.size = s.size;
  str.hasNull = 1;
  return str;
}

static const char *str8C(str8 s) { return (const char *)s.str; }
static const char *str8AC(MArena *a, str8 s) {
  str8 n = str8NullTerminate(a, s);
  return (const char *)n.str;
}

static b8 str8ReadF32(str8 token, f32 *result) {
  u32 mantissa = 0;
  i32 exponent = 0;
  b8 neg = 0;
  b8 dot = 0;
  for (i32 i = 0; i < token.size; i++) {
    u8 ch = token.str[i];
    if (ch == '-') {
      neg = 1;
      continue;
    }

    if (ch == '.') {
      if (dot) {
        goto error;
      }

      dot = 1;
      continue;
    }

    if (ch >= '0' && ch <= '9') {
      mantissa *= 10;
      mantissa += (ch - '0');
    }

    if (dot) {
      exponent -= 1;
    }
  }

  *result = f32(mantissa) * f32(pow(10, f32(exponent)));
  return 0;

error:
  LogError("emReadF32: failed to parse f32");
  return 1;
}

static b8 str8ReadU32(str8 token, u32 *result) {
  u32 mantissa = 0;
  i32 exponent = 0;
  b8 neg = 0;
  b8 dot = 0;
  for (i32 i = 0; i < token.size; i++) {
    u8 ch = token.str[i];
    if (ch == '-') {
      goto error;
    }

    if (ch == '.') {
      goto error;
    }

    if (ch >= '0' && ch <= '9') {
      mantissa *= 10;
      mantissa += (ch - '0');
    }
  }

  *result = mantissa;
  return 0;

error:
  LogError("emReadU32: failed to parse u32");
  return 1;
}

#endif