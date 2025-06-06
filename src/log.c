#include "../include/log.h"
#include <stdio.h>

void log_info(const char *msg) { printf("\033[1;34m[i]\033[0m %s\n", msg); }

void log_success(const char *msg) { printf("\033[1;32m[+]\033[0m %s\n", msg); }

void log_error(const char *msg) {
  fprintf(stderr, "\033[1;31m[!]\033[0m %s\n", msg);
}
