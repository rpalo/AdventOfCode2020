#include "Day5.h"

#include <stdio.h>
#include <stdbool.h>

#define ROWS 128
#define COLS 8
#define NUM_ROW_CHARS 7
#define NUM_COL_CHARS 3


int seat_ID(const char* seat) {
  int front = 0, left = 0;
  int depth = ROWS;
  int width = COLS;

  for (int i = 0; i < NUM_ROW_CHARS; i++) {
    depth >>= 1;
    if (seat[i] == 'B') front += depth;
  }

  for (int i = NUM_ROW_CHARS; i < NUM_ROW_CHARS + NUM_COL_CHARS; i++) {
    width >>= 1;
    if (seat[i] == 'R') left += width;
  }
  return front * 8 + left;
}


/// Part 1: Calculate the highest seat ID in the list.
static int part1(void) {
  FILE* fp;
  fp = fopen("data/day5.txt", "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  int max_ID = 0;
  char seat[11];

  while (fgets(seat, 11, fp)) {
    int this_ID = seat_ID(seat);
    if (this_ID > max_ID) max_ID = this_ID;
  }
  fclose(fp);
  return max_ID;
}

/// Part 2: Find the only empty seat on the plane.  Note: some seats
/// are missing from the front and back of the grid.
static int part2(void) {
  bool seats[ROWS*COLS] = {0};
  
  FILE* fp;
  fp = fopen("data/day5.txt", "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  // Load all seats in as present.
  char seat[11];
  while (fgets(seat, 11, fp)) {
    seats[seat_ID(seat)] = true;
  }
  fclose(fp);

  // Run through non-present seats.  Once we're into seats on the plane,
  // the first empty one is mine!
  bool inside_plane = false;
  for (int i = 0; i < ROWS*COLS; i++) {
    if (!inside_plane && seats[i]) inside_plane = true;
    if (inside_plane && !seats[i]) return i;
  }

  return -1;
}

int day5(void) {
  printf("====== Day 5 ======\n");
  printf("Part 1: %d\n", part1());
  printf("Part 2: %d\n", part2());
  return EXIT_SUCCESS;
}
