#ifndef AOC2020_DAY5_H
#define AOC2020_DAY5_H

/// Day 5: Binary Boarding
/// 
/// Calculate Seat ID's from a binary division process.

#include <stdlib.h>

/// Calculate the seat ID by parsing a 10-char FFBBFFBLRL seat string
/// to a seat row/column.  The seat ID is not only row * 8 + col, but
/// also the index into a linearized array of seats which is nice.
int seat_ID(const char* seat);

/// Run both parts for the day.
int day5(void);
#endif
