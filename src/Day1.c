/// Day 1: Report Repair
/// 
/// My expense report for my trip is messed up and entries need
/// adjusting.

#include "Day1.h"

#include <stdio.h>

/// Parse the input file, which contains one positive integer per line.
int* parse(const char* filename, size_t* count) {
  FILE *fp;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open input file.\n");
    exit(EXIT_FAILURE);
  }

  // Count how many we need
  size_t lines = 0;
  while (!feof(fp)) {
    if (fgetc(fp) == '\n') lines++;
  }
  lines++;  // The last line has no newline, but we should count it.
  rewind(fp);

  int* entries = malloc(sizeof(int)*lines);

  char buff[5];  // No numbers greater than 4 digits
  for (size_t i = 0; i < lines; i++) {
    fgets(buff, 6, fp);
    entries[i] = atoi(buff);
  }
  fclose(fp);

  *count = lines;
  return entries;
}

/// Part 1 finds the two entries that add up to 2020 exactly and returns
/// their product, or -1 if none are found.
int part1(int* entries, size_t count) {
  for (size_t i = 0; i < count; i++) {
    for (size_t j = i + 1; j < count; j++) {
      if (entries[i] + entries[j] == 2020) {
        return entries[i] * entries[j];
      }
    }
  }
  return -1;
}

/// Part 2 is the same as Part 1, but we're looking for a trio of numbers
/// that sum up to 2020.  Returns their product or -1 if no trio is found.
int part2(int* entries, size_t count) {
  for (size_t i = 0; i < count; i++) {
    for (size_t j = i + 1; j < count; j++) {
      for (size_t k = j + 1; k < count; k++) {
        if (entries[i] + entries[j] + entries[k] == 2020) {
          return entries[i] * entries[j] * entries[k];
        }
      }
    }
  }
  return -1;
}

int day1() {
  size_t count;
  int* entries = parse("data/day1.txt", &count);
  printf("====== Day 1 ======\n");
  printf("The 2020 pair product is: %d\n", part1(entries, count));
  printf("The 2020 triplet product is: %d\n", part2(entries, count));
  return EXIT_SUCCESS;
}