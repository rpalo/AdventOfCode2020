#ifndef AOC2020_PARSING_H
#define AOC2020_PARSING_H

#include <stdio.h>
#include <stdlib.h>

/// Counts the number of newline characters in a text file.
/// Assumes no newline at the end of the last line (so adds +1 more)
size_t count_lines(FILE* fp);

/// Counts the number of newline characters in a text file until it
/// reaches a blank line.  The resulting count does not include the
/// blank line.
size_t count_lines_until_blank(FILE* fp);

/// The height and width of a 2D grid.
typedef struct {
  size_t width;
  size_t height;
} GridSize;

/// Takes in a file containing a 2D grid and returns its width/height.
GridSize measure_grid(FILE* fp);

#endif