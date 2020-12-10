#include "Day9.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "parsing.h"

/// Day 9: Encoding Error
/// 
/// Find the number that can't be made by a combination of the numbers
/// preceding it.

#define MAX_NUMBER_STRING_SIZE 16

/// Parse the input file into a list of integers, one per line.
int* parse(const char* filename, int* count) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  *count = count_lines(fp);
  int* numbers = (int*)malloc(sizeof(int) * *count);
  for (int i = 0; i < *count; i++) {
    char buf[MAX_NUMBER_STRING_SIZE] = {0};
    fgets(buf, MAX_NUMBER_STRING_SIZE, fp);
    numbers[i] = atoi(buf);
  }
  fclose(fp);
  return numbers;
}

/// Decides if a number is a 'valid number,' i.e. there is some
/// combination of two numbers in the preceding 'preamble_length' 
/// numbers that add up to it.
bool valid_number(int* numbers, int i, int preamble_length) {
  // Add each pair of numbers in the preceding 'preamble_length' #'s
  // If there's any pair that add up to the i'th number, it's valid.
  for (int j = 0; j < preamble_length; j++) {
    for (int k = j + 1; k < preamble_length; k++) {
      if (numbers[i - preamble_length + j] + numbers[i - preamble_length + k] == numbers[i]) {
        return true;
      }
    }
  }
  return false;
}

/// Find the one number that isn't 'valid' per the above definition.
int part1(const char* filename, int preamble_length) {
  int count = 0;
  int* numbers = parse(filename, &count);

  for (int i = preamble_length; i < count; i++) {
    if (!valid_number(numbers, i, preamble_length)) {
      return numbers[i];
    }
  }
  return -1;
}

/// There is a set of contiguous numbers, two or greater in length,
/// that, when added up, equal the invalid number from part 1.
/// Return the minimum number in that set + the max in that set.
int part2(const char* filename, int preamble_length) {
  int count = 0;
  int* numbers = parse(filename, &count);
  
  int invalid_target = part1(filename, preamble_length);

  // Check each size from 2 all the way up to all of the numbers
  for (int size = 2; size <= count; size++) {

    // At each size, shift the window from front to back, one by one.
    for (int i = 0; i + size - 1 < count; i++) {

      // Add up that window of numbers and see if it matches
      int total = 0;
      for (int j = 0; j < size; j++) {
        total += numbers[i + j];
      }

      // If it matches, find the min and max values in that window
      // and return their sum.
      if (total == invalid_target) {
        int min = INT_MAX;
        int max = 0;

        // Simultaneously find the min and max in the range.
        for (int j = 0; j < size; j++) {
          if (numbers[i + j] > max) max = numbers[i + j];
          if (numbers[i + j] < min) min = numbers[i + j];
        }
        return min + max;
      }
    }
  }
  return -1;
}

/// Run both parts
int day9() {
  printf("====== Day 9 ======\n");
  printf("Part 1: %d\n", part1("data/day9.txt", 25));
  printf("Part 2: %d\n", part2("data/day9.txt", 25));
  return EXIT_SUCCESS;
}
