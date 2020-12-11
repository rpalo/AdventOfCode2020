#ifndef AOC2020_DAY11_H
#define AOC2020_DAY11_H

#include <stdbool.h>
#include <stdlib.h>

#include "parsing.h"

/// Potential options for a seat
typedef enum {
  SEAT_FLOOR,   ///< Seat gone
  SEAT_EMPTY,   ///< No one in seat
  SEAT_FULL,    ///< Seat occupied
} SeatType;

/// A grid of seats (1D, but to be indexed as 2D)
typedef struct {
  GridSize size;
  SeatType* cells;
} Grid;

Grid* parse(const char* filename);
int part1(const char* filename);
bool first_visible_full(SeatType* cells, int x, int y, int width, int height, int xdir, int ydir);
int part2(const char* filename);
int day11(void);
#endif
