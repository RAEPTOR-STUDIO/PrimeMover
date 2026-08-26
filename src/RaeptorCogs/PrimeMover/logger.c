#include "RaeptorCogs/Cog/function_registry.h"
#include <RaeptorCogs/Cog/info.h>
#include <RaeptorCogs/PrimeMover/logger.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void logger_log(FILE *out, const char *message) {
  ((void (*)(FILE *, const char *))fn_registry->get("logger_log"))(out,
                                                                   message);
};

void logger_logf(FILE *out, const char *message, ...) {
  va_list args;
  va_list copy;
  int len;
  char *buf;

  va_start(args, message);

  va_copy(copy, args);
  len = vsnprintf(NULL, 0, message, copy);
  va_end(copy);

  if (len < 0) {
    va_end(args);
    return;
  }

  buf = malloc((size_t)len + 1);
  if (!buf) {
    va_end(args);
    return;
  }

  vsnprintf(buf, (size_t)len + 1, message, args);
  va_end(args);

  logger_log(out, buf);
  free(buf);
};

void logger_log_fallback(FILE *out, const char *message) {
  static bool first_time_use = true;
  if (first_time_use) {
    first_time_use = false;
    fputs("\033[38;5;244m", stdout);
    fputs("You are using default logger. You may want to use a "
          "logger Cog.\n",
          stdout);
    fputs("\033[0m", stdout);
  }
  fputc('[', out);
  fputs(cog_name(), out);
  fputs("] ", out);
  fputs(message, out);
  fputc('\n', out);
}
