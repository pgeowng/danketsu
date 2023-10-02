#define AlignUpPow2(x, p) (((x) + (p)-1) & ~((p)-1))
#define AlignDownPow2(x, p) ((x) & ~((p)-1))
#define ArrayCount(a) (sizeof(a) / sizeof(*(a)))

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define ClampTop(a, b) Min(a, b)

#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) ((x) << 40)

#include <stdint.h>

typedef unsigned int uint;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t i32;
typedef int8_t i8;
typedef float f32;

#include <string.h>
#define MemoryZero(p, z) memset((p), 0, (z))

// typedef struct m4 {
//   f32 a[4 * 4];
// } m4;

typedef f32 *v2;
typedef f32 *v3;
typedef f32 *v4;
typedef f32 *m4;

// static inline v2Init

m4 m4Ortho(m4 m, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
  const f32 *expect =
      glm::value_ptr(glm::ortho(left, right, bottom, top, near, far));
  for (i32 i = 0; i < 16; i++) {
    m[i] = expect[i];
  }
  return m;
}

typedef struct logEvent {
  va_list ap;
  const char *fmt;
  const char *file;
  int line;
  int level;
} logEvent;

static logEvent newLogEvent(const char *fmt, const char *file, int line,
                            int level) {
  logEvent ev = {};
  ev.fmt = fmt;
  ev.file = file;
  ev.line = line;
  ev.level = ev.level;
  return ev;
}

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };
void LogLog(int level, const char *file, int line, const char *fmt, ...);

// https://c-faq.com/varargs/handoff.html
void vlog(logEvent *ev) {
  fprintf(stderr, "%d: %s:%d: ", ev->level, ev->file, ev->line);
  vfprintf(stderr, ev->fmt, ev->ap);
  fprintf(stderr, "\n");
  fflush(stderr);
}

void LogLog(int level, const char *file, int line, const char *fmt, ...) {
  logEvent ev = newLogEvent(fmt, file, line, level);
  va_start(ev.ap, fmt);
  vlog(&ev);
  va_end(ev.ap);
}

#define LogError(...) LogLog(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)