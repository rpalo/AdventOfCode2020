/// Day 3: Taboggan Trajectory
/// 
/// Figure out how many trees you'll have to go past given a 2D grid/slope
/// covered in them.

#include "Day3.h"
#include "parsing.h"

#include <stdio.h>

TreeGrid* parse(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open input file.\n");
    exit(EXIT_FAILURE);
  }

  TreeGrid* grid = malloc(sizeof(TreeGrid));
  GridSize size = measure_grid(fp);
  grid->cells = malloc(sizeof(TerrainType) * size.width * size.height);

  char c;
  for (size_t i = 0; !feof(fp);) {
    switch (c = getc(fp)) {
    case '.':
      grid->cells[i] = OPEN;
      i++;
      break;
    case '#':
      grid->cells[i] = TREE;
      i++;
      break;
    case '\n':
    case EOF:
      // Just consume the newline
      break;
    default:
      printf("Unrecognized character on char (x=%zu, y=%zu): %c.\n",
      i % size.width,
      i / size.width,
      c);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fp);
  grid->height = size.height;
  grid->width = size.width;
  return grid;
}

void freeTreeGrid(TreeGrid* grid) {
  free(grid->cells);
  grid->cells = NULL;
  free(grid);
}

/// Calculates how many trees you'll see on a linear trip down the hill.
/// If you go off the right end of the grid, wraps around infinitely wide.
static size_t calculateTrees(TreeGrid* grid, size_t xShift, size_t yDrop) {
  size_t trees = 0;
  for (size_t x = 0, y = 0; y < grid->height; y += yDrop, x = (x + xShift) % grid->width) {
    switch (CELL(grid, x, y)) {
    case TREE:
      trees++;
      break;
    case OPEN:
      break;
    }
  }
  return trees;
}

size_t part1(TreeGrid* grid) {
  return calculateTrees(grid, 3, 1);
}

size_t part2(TreeGrid* grid) {
  return calculateTrees(grid, 1, 1) *
         calculateTrees(grid, 3, 1) *
         calculateTrees(grid, 5, 1) *
         calculateTrees(grid, 7, 1) *
         calculateTrees(grid, 1, 2);
}

int day3() {
  TreeGrid* grid = parse("data/day3.txt");
  printf("====== Day 3 ======\n");
  printf("Part 1: %zu\n", part1(grid));
  printf("Part 2: %zu\n", part2(grid));

  freeTreeGrid(grid);
  return EXIT_SUCCESS;
}
