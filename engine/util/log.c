#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

void log_debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, "[DEB] ", args);
    vfprintf(stdout, fmt, args);
    vfprintf(stdout, "\n", args);
    va_end(args);
    fflush(stdout);
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, "[ERR] ", args);
    vfprintf(stderr, fmt, args);
    vfprintf(stderr, "\n", args);
    va_end(args);
    fflush(stdout);
}

#endif