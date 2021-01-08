#include "Day23.h"

#include <stdio.h>

#define PART1_MAX_CUPS 9
#define PART1_ITERATIONS 100
#define PART2_MAX_CUPS 1000000
#define PART2_ITERATIONS 10000000

/// A Cup is a node in a cyclic linked list consisting of a number
/// and a pointer to the next node.
typedef struct Cup {
  int value;
  struct Cup* next;
} Cup;

/// Part one parses the digits in the input into 9 distinct cups
/// and links tail to head to form a cycle.  Returning the head
/// of the cycle is sufficient.
Cup* parse(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }
  Cup* head = (Cup*)malloc(sizeof(Cup));
  head->value = getc(fp) - '0';

  Cup* current = head;
  char c;
  while ((c = getc(fp)) != EOF) {
    current->next = (Cup*)malloc(sizeof(Cup));
    current = current->next;
    current->value = c - '0';
  }
  current->next = head;
  fclose(fp);
  return head;
}

/// Part one runs 100 iterations of the following algorithm:
///
/// 1. Detatch the three cups immediately clockwise (next) of the
///    current cup.
/// 2. Select a target number that is 1 less than the current cup's.
///    If that target number is one of the selected cups's numbers,
///    subtract 1 again.  If we go to 0, start over at the top end
///    of the set of cups.
/// 3. Insert the detatched 3 cups immediately after the cup with its
///    value equal to the target number.
/// 4. Increment the current cup to the next clockwise cup.
///
/// Finally, returns the digits in-order starting with the one immediately
/// clockwise from 1 and stopping just before 1 at the end.
int part1(const char* filename) {
  Cup* current = parse(filename);

  for (int i = 0; i < PART1_ITERATIONS; i++) {
    Cup* removed_head = current->next;
    Cup* removed_tail = removed_head->next->next;
    current->next = removed_tail->next;
    int dest = current->value - 1;
    while (dest == removed_head->value || dest == removed_head->next->value || dest == removed_tail->value || dest == 0) {
      dest -= 1;
      if (dest <= 0) dest = PART1_MAX_CUPS;
    }
    Cup* dest_cup = current;
    // Linear search for the destination cup.
    for (int count = 0; count < PART1_MAX_CUPS + 1; count++, dest_cup = dest_cup->next) {
      if (dest_cup->value == dest) {
        removed_tail->next = dest_cup->next;
        dest_cup->next = removed_head;
        break;
      }
      if (count == PART1_MAX_CUPS) {
        printf("Didn't find the dest cup.  Dest: %d\n", dest);
        exit(EXIT_FAILURE);
      }
    }
    current = current->next;
  }
  // Linear search for 1.
  while (current->value != 1) current = current->next;

  // Create a number that is the next 8 digits following 1.
  current = current->next;
  int result = 0;
  while (current->value != 1) {
    result *= 10;
    result += current->value;
    current = current->next;
  }

  return result;
}

/// The second part parsing function does much the same thing as part 1,
/// but, additionally, we store an array of cups, with each cup stored
/// at the index of its value.  That way, lookup cost is O(1) instead
/// of O(n).
Cup** parse2(const char* filename, int* start_index) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  // Store the cups *at* their index, which means the 0th spot is
  // empty for ergonomic reasons.
  Cup** cups = (Cup**)malloc(sizeof(Cup*)*(PART2_MAX_CUPS + 1));
  if (cups == NULL) {
    printf("Couldn't malloc the cups array.\n");
    exit(EXIT_FAILURE);
  }
  Cup* head = (Cup*)malloc(sizeof(Cup));
  head->value = getc(fp) - '0';
  cups[head->value] = head;

  // We still have to know which cup is our starting cup, so save that
  *start_index = head->value;

  // Add the rest of the cups in the order specified in the input file.
  Cup* current = head;
  char c;
  while ((c = getc(fp)) != EOF) {
    current->next = (Cup*)malloc(sizeof(Cup));
    current = current->next;
    current->value = c - '0';
    cups[current->value] = current;
  }
  
  // Add the remaining cups from 10 to 
  for (int i = PART1_MAX_CUPS + 1; i <= PART2_MAX_CUPS; i++) {
    current->next = (Cup*)malloc(sizeof(Cup));
    current = current->next;
    current->value = i;
    cups[i] = current;
  }
  current->next = head;
  fclose(fp);
  return cups;
}

/// Free the cups
static void free_cups(Cup** cups) {
  for (int i = 1; i <= PART2_MAX_CUPS; i++) {
    free(cups[i]);
  }
  free(cups);
}

/// Part two upgrades to 1,000,000 cups and 10,000,000 iterations.
/// Returns the product of the two cups immediately following 1 afterwards.
int64_t part2(const char* filename) {
  int start_index;
  Cup** cups = parse2(filename, &start_index);
  Cup* current = cups[start_index];

  for (int i = 0; i < PART2_ITERATIONS; i++) {
    Cup* removed_head = current->next;
    Cup* removed_tail = removed_head->next->next;
    current->next = removed_tail->next;
    int dest = current->value - 1;
    while (dest == removed_head->value || dest == removed_head->next->value || dest == removed_tail->value || dest == 0) {
      dest -= 1;
      if (dest <= 0) dest = PART2_MAX_CUPS;
    }
    Cup* dest_cup = cups[dest];
    removed_tail->next = dest_cup->next;
    dest_cup->next = removed_head;
    current = current->next;
  }
  int64_t result = (int64_t)cups[1]->next->value * (int64_t)cups[1]->next->next->value;
  free_cups(cups);
  return result;
}

/// Run both parts.
int day23() {
  printf("====== Day 23 ======\n");
  printf("Part 1: %d\n", part1("data/day23.txt"));
  printf("Part 2: %lld\n", part2("data/day23.txt"));
  return EXIT_SUCCESS;
}
