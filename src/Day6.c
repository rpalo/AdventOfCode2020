#include "Day6.h"

#include <stdio.h>
#include <string.h>

/// Number of letters in the alphabet
#define LETTERS 26

/// Max number of chars per line (yes to all plus newline plus NULL)
#define MAX_LINE_SIZE (LETTERS + 2)

int tally_anyone_yeses(FILE* fp) {
  int counts[LETTERS] = {0};
  char line[MAX_LINE_SIZE] = {0};
  
  // Log each occurrence of a character.
  while (!feof(fp)) {
    fgets(line, MAX_LINE_SIZE, fp);
    if (line[0] == '\n') break;

    for (int i = 0; line[i] != '\n' && line[i]; i++) {
      int index = line[i] - 'a';
      counts[index]++;
    }
    memset(line, 0, MAX_LINE_SIZE);
  }

  // Loop through and find the total count of those that appeared.
  int total = 0;
  for (int i = 0; i < LETTERS; i++) {
    if (counts[i] > 0) total++;
  }
  return total;
}

/// Run the part 1 code on the input file.
static int part1() {
  FILE* fp;
  fp = fopen("data/day6.txt", "r");
  if (fp == NULL) {
      printf("Couldn't open input file.\n");
      exit(EXIT_FAILURE);
  }

  // Loop through and add up the total for each group.
  int total = 0;
  while (!feof(fp)) {
      total += tally_anyone_yeses(fp);
  }

  fclose(fp);
  return total;
}

int tally_everyone_yeses(FILE* fp) {
  int counts[LETTERS] = {0};
  char line[MAX_LINE_SIZE] = {0};
  int people_count = 0;

  // Loop through the group and add up everybody's answers.
  while (!feof(fp)) {
    fgets(line, MAX_LINE_SIZE, fp);
    if (line[0] == '\n') break;
    
    people_count++;
    for (int i = 0; line[i] != '\n' && line[i]; i++) {
      int index = line[i] - 'a';
      counts[index]++;
    }
    memset(line, 0, MAX_LINE_SIZE);
  }

  // Count the number of letters where everyone answered it yes
  // i.e. number of yeses == number of people
  int total = 0;
  for (int i = 0; i < LETTERS; i++) {
    if (counts[i] == people_count) total++;
  }
  return total;
}

/// Run the part 2 code on the input file.
int part2() {
  FILE* fp;
  fp = fopen("data/day6.txt", "r");
  if (fp == NULL) {
      printf("Couldn't open input file.\n");
      exit(EXIT_FAILURE);
  }

  // Loop through and add up the total for each group.
  int total = 0;
  while (!feof(fp)) {
      total += tally_everyone_yeses(fp);
  }

  fclose(fp);
  return total;
}

int day6() {
  printf("====== Day 6 ======\n");
  printf("Part 1: %d\n", part1());
  printf("Part 2: %d\n", part2());
  return EXIT_SUCCESS;
}
