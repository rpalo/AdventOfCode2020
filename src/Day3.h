#ifndef AOC2020_DAY3_H
#define AOC2020_DAY3_H

#include <stdlib.h>

/// All of the possible options for a terrain square in a TreeGrid.
/// OPEN has no trees in it.
/// TREE has a tree in it.
typedef enum {
  OPEN,
  TREE,
} TerrainType;

/// A 2D grid of terrain.
/// Knows how wide and tall it is.
/// Uses a 1D list of cells, and tricky math to index.
typedef struct {
  TerrainType* cells;
  size_t width;
  size_t height;
} TreeGrid;

/// 2D indexing into a TreeGrid
#define CELL(t, x, y) ((t)->cells[y*(t)->width + x])

/// Parse the input file, which is a 2D grid of '.' (free) and '#' (tree)
/// Return a pointer to a TreeGrid.
TreeGrid* parse(const char* filename);

/// Frees a TreeGrid.
void freeTreeGrid(TreeGrid* grid);

/// Part 1 calculates how many trees we'll hit with a slope of 3 right
/// and 1 down.
size_t part1(TreeGrid* grid);

/// Part 2 has us check a few different slopes for trees.  The result
/// is the product of all tree counts.
size_t part2(TreeGrid* grid);

/// Runs both parts and outputs the results.
int day3(void);
#endif
