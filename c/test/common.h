#ifndef COMMON_H
#define COMMON_H

#include <stdarg.h>
#include <stdio.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define BOLD "\033[1m"

static inline void log_info(int step, const char *color, const char *fmt, ...) {
  printf(BOLD YELLOW "%02d " RESET, step);
  printf("%s", color);

  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);

  printf(RESET);
}

#define LOG_GREEN(step, fmt, ...) log_info(step, BOLD GREEN, fmt, ##__VA_ARGS__)
#define LOG_BLUE(step, fmt, ...) log_info(step, BOLD BLUE, fmt, ##__VA_ARGS__)
#define LOG_RED(step, fmt, ...) log_info(step, BOLD RED, fmt, ##__VA_ARGS__)

#endif
