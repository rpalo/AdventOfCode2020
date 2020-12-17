#include "parsing.h"

#include <stdlib.h>
#include <stdio.h>

size_t count_lines(FILE* fp) {
  size_t lines = 0;
  for (;;) {
    char c = getc(fp);
    if (c == '\n') lines++;
    if (c == EOF) break;
  }

  rewind(fp);
  return lines + 1;
}

size_t count_lines_until_blank(FILE* fp) {
  size_t lines = 0;
  for(;;) {
    char c = getc(fp);
    if (c == '\n') {
      lines++;
      if (getc(fp) == '\n') break;
    }
    if (c == EOF) break;
  }

  rewind(fp);
  return lines;
}

GridSize measure_grid(FILE* fp) {
  size_t lines = 0;
  size_t cols = 0;

  while (getc(fp) != '\n') cols++;
  lines++;
  while (!feof(fp)) {
    if (getc(fp) == '\n') lines++;
  }
  lines++; // Assume no newline after last line.

  rewind(fp);
  return (GridSize) {.width = cols, .height = lines};
}