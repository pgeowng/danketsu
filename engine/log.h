#include "cstdio"

#ifndef LOG_H
#define LOG_H

void log_error(const char *msg, const char *file, int line);

void log_error(const char *msg, const char *file, int line) {
  printf("%s:%d: error: %s\n", file, line, msg);
}

void log_debug(const char *msg, const char *file, int line);

void log_debug(const char *msg, const char *file, int line) {
  printf("%s:%d: debug: %s\n", file, line, msg);
}

#define LOG_ERROR(msg) log_error(msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg) log_debug(msg, __FILE__, __LINE__)

#endif