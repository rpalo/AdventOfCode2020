#include "parsing.h"

#include <stdlib.h>
#include <stdio.h>

size_t count_lines(FILE* fp) {
  size_t lines = 0;
  while (!feof(fp)) {
    if (getc(fp) == '\n') lines++;
  }

  rewind(fp);
  return lines + 1;
}