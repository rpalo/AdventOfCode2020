#ifndef AOC2020_PARSING_H
#define AOC2020_PARSING_H

#include <stdio.h>
#include <stdlib.h>

/// Counts the number of newline characters in a text file.
/// Assumes no newline at the end of the last line (so adds +1 more)
size_t count_lines(FILE* fp);

typedef struct {
  size_t width;
  size_t height;
} GridSize;

GridSize measure_grid(FILE* fp);

#endif