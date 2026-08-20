#pragma once
#include <stdio.h>

void logger_log(FILE *out, const char *message);
void logger_logf(FILE *out, const char *message, ...);

void logger_log_fallback(FILE *out, const char *message);
