#include "Day13.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/// Day 13: Shuttle Search
///
/// Plan your trip based on regularly timed intersecting shuttle departures.

/// A simple bus schedule for a fleet of busses.
typedef struct {
  int earliest;   ///< The earliest possible timestep you could depart
  int* ids;       ///< List of Bus ID's (which are also their frequencies)
  int count;      ///< The number of busses
} BusSchedule;

/// Parse the input file:
/// First line is just the earliest timestep we could leave.
/// Second line consists of integer bus ID's separated by commas
BusSchedule parse(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  BusSchedule schedule = {0};
  fscanf(fp, "%d\n", &schedule.earliest);
  
  char line[255] = {0};
  fscanf(fp, "%s", line);
  fclose(fp);

  // Count and allocate the ID's
  int count = 0;
  for (int i = 0; line[i]; i++) {
    if (line[i] == ',') count++;
  }
  count++;
  schedule.ids = (int*)malloc(sizeof(int) * count);

  // Split, parse, and store them.
  int i = 0;
  char* token = strtok(line, ",");
  while (token != NULL) {
    schedule.ids[i] = atoi(token);
    token = strtok(NULL, ",");
    i++;
  }
  schedule.count = count;
  return schedule;
}

void free_bus_schedule(BusSchedule* schedule) {
  free(schedule->ids);
  // Don't free schedule because it's on the stack
}

/// What is the ID of the bus who leaves soonest after you get there
/// multiplied by the amount of minutes you'll be waiting for it?
int part1(const char* filename) {
  BusSchedule schedule = parse(filename);

  int best_bus_id = 0;
  int best_wait_time = INT_MAX;

  for (int i = 0; i < schedule.count; i++) {
    if (schedule.ids[i] == 0) continue;
    int wait =  schedule.ids[i] - (schedule.earliest % schedule.ids[i]);
    
    if (wait < best_wait_time) {
      best_wait_time = wait;
      best_bus_id = schedule.ids[i];
    }
  }

  free_bus_schedule(&schedule);
  return best_bus_id * best_wait_time;
}

/// A bus schedule for the contest to see who can calculate the timestep
/// where the first bus ID leaves and every subsequent nth bus leaves
/// on the subsequent nth timestep.  X's don't matter.
typedef struct {
  int* ids;       ///< List of the bus id's
  int* offsets;   ///< List of time offsets (i.e. bus `i` should leave at time `t + i`)
  int count;      ///< Number of busses (that we care about, not X's)
} ContestBusSchedule;

/// Parse the input file (same format as part 1) into a ContestBusSchedule.
ContestBusSchedule parse2(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  ContestBusSchedule schedule = {0};
  char nonsense[25];
  fgets(nonsense, 25, fp);
  
  char line[255] = {0};
  fscanf(fp, "%s", line);
  fclose(fp);

  // Count and allocate the ID's
  int count = 0;
  for (int i = 0; line[i]; i++) {
    if (line[i] == ',' && line[i-1] != 'x') count++;
  }
  count++;
  schedule.count = count;
  schedule.ids = (int*)malloc(sizeof(int) * count);
  schedule.offsets = (int*)malloc(sizeof(int) * count);

  // Split, parse, and store them.
  int i = 0;
  int offset = 0;
  char* token = strtok(line, ",");
  while (token != NULL) {
    if (*token == 'x') {
      token = strtok(NULL, ",");
      offset++;
      continue;
    }
    schedule.ids[i] = atoi(token);
    schedule.offsets[i] = offset;
    token = strtok(NULL, ",");
    offset++;
    i++;
  }
  
  return schedule;
}

/// Find the earliest time `t` that satisfies the requirements discussed
/// above in the `ContestBusSchedule`.
///
/// The trick is that for every [0..i] subset of the numbers, their intersection
/// repeats every (PRODUCT(numbers[0..i])) minutes.  So my algorithm
/// is to roll it up:
/// Start by finding the intersection of the first two numbers where
/// bus 0 departs at t and bus 1 departs at t + 1.  From then on,
/// a timestep of bus0 * bus1 will guarantee that all times conform
/// to the same relationship between bus0 and bus1.  Then you can look for
/// an instance where bus2 appears at t + 2.  Then adjust timestep to
/// bus0 * bus1 * bus2.
///
/// Lather, rinse, repeat, continuously growing your timestep so that
/// you're always only really looking for one more number to line up
/// which should be pretty quick.
///
/// You're only guaranteed the earliest `t` because all numbers in the
/// input file are prime.  Otherwise, it could happen a multiple of one of
/// the non-prime buses earlier.
long part2(const char* filename) {
  ContestBusSchedule schedule = parse2(filename);
  
  long step = schedule.ids[0];
  int search_idx = 1;
  for (long t = step; t < LONG_MAX; t += step) {
    bool success = true;
    for (int i = 0; i <= search_idx; i++) {
      if ((t + schedule.offsets[i]) % schedule.ids[i] != 0) {
        success = false;
        break;
      }
    }
    if (success && search_idx == schedule.count - 1) return t;
    if (success) {
      step *= schedule.ids[search_idx];
      search_idx++;
    }
  }
  return -1;
}

/// Run both parts.
int day13() {
  printf("====== Day 13 ======\n");
  printf("Part 1: %d\n", part1("data/day13.txt"));
  printf("Part 2: %ld\n", part2("data/day13.txt"));
  return EXIT_SUCCESS;
}
