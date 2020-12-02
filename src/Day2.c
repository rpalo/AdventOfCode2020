/// Day 2: Password Philosophy 
/// 
/// Find the passwords that aren't compliant.

#include "Day2.h"
#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/// Parse the input file,
static PoliciedPassword** parse(size_t* lines) {
  FILE* fp;
  fp = fopen("data/day2.txt", "r");
  if (fp == NULL) {
    printf("Couldn't open input file.\n");
    exit(EXIT_FAILURE);
  }

  *lines = count_lines(fp);

  PoliciedPassword** passes = malloc(sizeof(PoliciedPassword*) * *lines);

  for (size_t i = 0; i < *lines; i++) {
    PoliciedPassword* p = malloc(sizeof(PoliciedPassword));
    fscanf(fp, "%d-%d %c: %s\n", &p->a, &p->b, &p->letter, p->password);
    passes[i] = p;
  }

  fclose(fp);
  return passes;
}

static void freePoliciedPasswordList(PoliciedPassword** passes, size_t count) {
  for (size_t i = 0; i < count; i++) {
    free(passes[i]);
    passes[i] = NULL;
  }
  free(passes);
}

int part1(PoliciedPassword** passes, size_t count) {
  size_t valid = 0;

  for (size_t i = 0; i < count; i++) {
    PoliciedPassword* p = passes[i];
    uint8_t matches = 0;
    for (size_t j = 0; p->password[j]; j++) {
      if (p->password[j] == p->letter) matches++;
    }
    if (p->a <= matches && matches <= p->b) valid++;
  }
  return valid;
}

int part2(PoliciedPassword** passes, size_t count) {
  size_t valid = 0;

  for (size_t i = 0; i < count; i++) {
    PoliciedPassword* p = passes[i];
    int matches = 0;
    if (p->password[p->a - 1] == p->letter) matches++;
    if (p->password[p->b - 1] == p->letter) matches++;
    if (matches == 1) valid++;
  }
  return valid;
}

int day2() {
  size_t count;
  PoliciedPassword** passes = parse(&count);
  printf("====== Day 2 ======\n");
  printf("Part 1: %d\n", part1(passes, count));
  printf("Part 2: %d\n", part2(passes, count));
  freePoliciedPasswordList(passes, count);
  return EXIT_SUCCESS;
}