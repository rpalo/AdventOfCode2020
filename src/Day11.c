#include "Day11.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "parsing.h"

/// Parse the input file, a 2D grid of '.', '#', or 'L'.
Grid* parse(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open the file!\n");
    exit(EXIT_FAILURE);
  }

  GridSize size = measure_grid(fp);
  SeatType* cells = (SeatType*)malloc(sizeof(SeatType) * size.height * size.width);

  for (int y = 0; y < size.height; y++) {
    for (int x = 0; x < size.width; x++) {
      switch (getc(fp)) {
        case '.': cells[y * size.width + x] = SEAT_FLOOR; break;
        case 'L': cells[y * size.width + x] = SEAT_EMPTY; break;
        case '#': cells[y * size.width + x] = SEAT_FULL; break;
        default: printf("Bad char.\n"); exit(EXIT_FAILURE);
      }
    }
    getc(fp);  // Burn newline
  }

  fclose(fp);
  Grid* grid = (Grid*)malloc(sizeof(Grid));
  grid->size = size;
  grid->cells = cells;
  return grid;
}

/// Free a grid's memory
static void free_grid(Grid* grid) {
  free(grid->cells);
  free(grid);
}

/// Change a grid one timestep based on the rules from part 1
///
/// 1. An empty seat with no occupied seats around it gets filled.
/// 2. A full seat with >= 4 occupied neighbors gets departed.
/// 3. Floor... floor never changes.
static bool evolve_grid(Grid* grid) {
  bool changed = false;

  SeatType new_cells[grid->size.height * grid->size.width];
  SeatType* cells = grid->cells;
  GridSize size = grid->size;

  /// Check if a given x, y coordinate seat is occupied
  #define FULL(_x, _y) (cells[(_y)*size.width + _x] == SEAT_FULL)

  for (int y = 0; y < grid->size.height; y++) {
    for (int x = 0; x < grid->size.width; x++) {
      int idx = y * size.width + x;
      if (cells[idx] == SEAT_FLOOR) {
        new_cells[idx] = SEAT_FLOOR;  // Floor never changes, short circuit
        continue;
      } 

      int occupied = 0;
      if (y > 0 && FULL(x, y - 1)) occupied++;
      if (y < size.height - 1 && FULL(x, y + 1)) occupied++;
      if (x > 0 && FULL(x - 1, y)) occupied++;
      if (x < size.width - 1 && FULL(x + 1, y)) occupied++;
      if (y > 0 && x > 0 && FULL(x - 1, y - 1)) occupied++;
      if (y > 0 && x < size.width - 1 && FULL(x + 1, y - 1)) occupied++;
      if (y < size.height - 1 && x > 0 && FULL(x - 1, y + 1)) occupied++;
      if (y < size.height - 1 && x < size.width - 1 && FULL(x + 1, y + 1)) occupied++;

      if (cells[idx] == SEAT_EMPTY && occupied == 0) {
        new_cells[idx] = SEAT_FULL;
        changed = true;
      } else if (cells[idx] == SEAT_FULL && occupied >= 4) {
        new_cells[idx] = SEAT_EMPTY;
        changed = true;
      } else {
        new_cells[idx] = cells[idx];
      }
    }
  }
  #undef FULL

  memcpy(grid->cells, new_cells, sizeof(SeatType) * size.width * size.height);
  return changed;
}

/// Check to see if the first visible chair in a given direction is full or not.
/// Skip over floor spaces, and stop at the edge of the grid.
bool first_visible_full(SeatType* cells, int x, int y, int width, int height, int xdir, int ydir) {
  int px = x + xdir;
  int py = y + ydir;
  SeatType val = SEAT_FLOOR;
  while (px >= 0 && py >= 0 && px < width && py < height && 
        (val = cells[py * width + px]) == SEAT_FLOOR) {
    px += xdir;
    py += ydir;
  }
  return (val == SEAT_FULL);
}

/// Change a grid one timestep based on the rules from part 2
///
/// 1. An empty seat with no occupied seats around it gets filled.
/// 2. A full seat with >= 5 occupied neighbors gets departed.
/// 3. Floor... floor never changes.
///
/// For this one, "around it" means the first seat in that direction
/// that isn't floor.
static bool evolve_grid2(Grid* grid) {
  bool changed = false;

  SeatType new_cells[grid->size.height * grid->size.width];
  SeatType* cells = grid->cells;
  GridSize size = grid->size;

  /// Convenience macro for checking first_visible_full in a given direction
  #define SEE_FULL(xdir, ydir) \
    (first_visible_full(cells, x, y, size.width, size.height, xdir, ydir))

  for (int y = 0; y < grid->size.height; y++) {
    for (int x = 0; x < grid->size.width; x++) {
      int idx = y * size.width + x;
      if (cells[idx] == SEAT_FLOOR) {
        new_cells[idx] = SEAT_FLOOR; // Floor never changes
        continue;
      } 

      int occupied = 0;
      if (SEE_FULL(-1, -1)) occupied++;
      if (SEE_FULL(-1, 0)) occupied++;
      if (SEE_FULL(-1, 1)) occupied++;
      if (SEE_FULL(0, -1)) occupied++;
      if (SEE_FULL(0, 1)) occupied++;
      if (SEE_FULL(1, -1)) occupied++;
      if (SEE_FULL(1, 0)) occupied++;
      if (SEE_FULL(1, 1)) occupied++;
      
      if (cells[idx] == SEAT_EMPTY && occupied == 0) {
        new_cells[idx] = SEAT_FULL;
        changed = true;
      } else if (cells[idx] == SEAT_FULL && occupied >= 5) {
        new_cells[idx] = SEAT_EMPTY;
        changed = true;
      } else {
        new_cells[idx] = cells[idx];
      }
    }
  }

  #undef SEE_FULL

  memcpy(grid->cells, new_cells, sizeof(SeatType) * size.width * size.height);
  return changed;
}

/*
static void print_grid(Grid* grid) {
  printf("\n");
  for (int y = 0; y < grid->size.height; y++) {
    for (int x = 0; x < grid->size.width; x++) {
      switch (grid->cells[y * grid->size.width + x]) {
        case SEAT_EMPTY: printf("L"); break;
        case SEAT_FLOOR: printf("."); break;
        case SEAT_FULL: printf("#"); break;
        default: printf("Bad seat type.\n"); exit(EXIT_FAILURE);
      }
    }
    printf("\n");
  }
}
*/

/// Part one, how many seats occupied after steady state
/// given the rules above?
int part1(const char* filename) {
  Grid* grid = parse(filename);

  while (evolve_grid(grid)) {}
  int occupied = 0;
  int size = grid->size.height * grid->size.width;
  for (int i = 0; i < size; i++) {
    if (grid->cells[i] == SEAT_FULL) occupied++;
  }
  
  free_grid(grid);
  return occupied;
}

/// Part two, how many seats occupied after steady state given
/// the rules above?
int part2(const char* filename) {
  Grid* grid = parse(filename);

  while (evolve_grid2(grid)) {}
  int occupied = 0;
  int size = grid->size.height * grid->size.width;
  for (int i = 0; i < size; i++) {
    if (grid->cells[i] == SEAT_FULL) occupied++;
  }
  
  free_grid(grid);
  return occupied;
}

/// Run both parts
int day11() {
  printf("====== Day 11 ======\n");
  printf("Part 1: %d\n", part1("data/day11.txt"));
  printf("Part 2: %d\n", part2("data/day11.txt"));
  return EXIT_SUCCESS;
}
