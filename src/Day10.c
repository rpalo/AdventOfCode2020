#include "Day10.h"

#include <stdio.h>

#include "parsing.h"

/// Day 10: Adapter Array
///
/// Use power adapters to calculate "joltage" jumps from connection
/// to connection.

/// In a sorted array of adapters, return the number of jumps of size 1
/// multiplied by the jumps of size 3.

/// Parse the input file, one number per line into a list of integers.
static int* parse(const char* filename, int* count) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  *count = count_lines(fp);
  
  // It's going to be easier if we manually attach our outlet and device
  *count += 2;
  int* numbers = (int*)malloc(sizeof(int) * *count);

  numbers[0] = 0;
  numbers[*count - 1] = 4 * *count; // Something so huge goes to the end.
  for (int i = 1; i < *count - 1; i++) {
    char buf[5] = {0};
    fgets(buf, 5, fp);
    numbers[i] = atoi(buf);
  }

  fclose(fp);
  return numbers;
}

/// Integer comparison function for qsort.
int cmp(const void* a, const void* b) {
  return (*(int*)a - *(int*)b);
}

int part1(const char* filename) {
  int count = 0;
  int* numbers = parse(filename, &count);

  qsort(numbers, count, sizeof(int), cmp);
  numbers[count - 1] = numbers[count - 2] + 3;

  int ones = 0;
  int threes = 0;

  for (int i = 1; i < count; i++) {
    int jump = numbers[i] - numbers[i - 1];
    if (jump == 1) ones++;
    else if (jump == 3) threes++;
  }

  free(numbers);
  return ones * threes;
}

/// How many possibilities does a group of continuous ones yield?
static int pattern_to_variants(int ones) {
  switch (ones) {
    case 0: return 1;
    case 1: return 1;
    case 2: return 2;
    case 3: return 4;
    case 4: return 7;
    default:
      printf("Crap, didn't account for > 4 ones.\n");
      exit(EXIT_FAILURE);
  }
}

/// Count the number of distinct arrangements of adapters that
/// would still work.
long long int part2(const char* filename) {
  int count = 0;
  int* numbers = parse(filename, &count);

  qsort(numbers, count, sizeof(int), cmp);
  numbers[count - 1] = numbers[count - 2] + 3;
  
  int ones = 0;
  long long int product = 1;

  for (int i = 1; i < count; i++) {  
    int jump = numbers[i] - numbers[i - 1];
    if (jump == 1) {
      ones++;
    } else if (jump == 3) {
      product *= pattern_to_variants(ones);
      ones = 0;
    } else {
      printf("Crap, this doesn't work for jumps of 2.\n");
      exit(EXIT_FAILURE);
    }
  }
  return product;
}

/// Run both parts.
int day10() {
  printf("====== Day 10 ======\n");
  printf("Part 1: %d\n", part1("data/day10.txt"));
  printf("Part 2: %lli\n", part2("data/day10.txt"));
  return EXIT_SUCCESS;
}
