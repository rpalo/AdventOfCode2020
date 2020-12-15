#include "Day14.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "parsing.h"
#include "hashmap.h"

typedef enum {
  MEM_MASK,
  MEM_SET,
} MemCommandType;

typedef struct {
  MemCommandType type;
  union {
    uint64_t and_mask;
    uint64_t address;
  };
  union {
    uint64_t or_mask;
    uint64_t value;
  };
} MemCommand;

void print_mask(uint64_t num) {
  for (int i = 35; i >= 0; i--) {
    printf("%d", (num >> i) & 1);
  }
  printf("\n");
}

static MemCommand parse_mask(const char* line) {
  MemCommand command = {0};
  command.type = MEM_MASK;
  
  for (int i = 7; line[i]; i++) {
    switch (line[i]) {
      case 'X': continue;
      case '0': command.and_mask |= 1 << (35 - i + 7); break;
      case '1': command.or_mask |= 1 << (35 - i + 7); break;
      case '\n': break;
      default: printf("Bad digit '%c'\n", line[i]); exit(EXIT_FAILURE);
    }
  }
  command.and_mask = (~command.and_mask) & 0xFFFFFFFFF;
  return command;
}

static MemCommand parse_set(const char* line) {
  MemCommand command = {0};
  command.type = MEM_SET;

  sscanf(line, "mem[%llu] = %llu", &command.address, &command.value);
  return command;
}

MemCommand* parse(const char* filename, int* count) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open input file.\n");
    exit(EXIT_FAILURE);
  }

  *count = count_lines(fp);
  MemCommand* commands = (MemCommand*)malloc(sizeof(MemCommand) * *count);

  for (int i = 0; i < *count; i++) {
    char line[100] = {0};
    fgets(line, 100, fp);
    if (line[1] == 'a') commands[i] = parse_mask(line);
    else if (line[1] == 'e') commands[i] = parse_set(line);
    else {
      printf("Bad line: '%s'\n", line);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fp);
  return commands;
}

uint64_t part1(const char* filename) {
  int count = 0;
  MemCommand* commands = parse(filename, &count);

  HashMap* memory = hashmap_create();
  MemCommand mask;

  for (int i = 0; i < count; i++) {
    MemCommand c = commands[i];
    switch (c.type) {
      case MEM_MASK: mask = c; break;
      case MEM_SET: {
        uint64_t new_value = (c.value & mask.and_mask) | mask.or_mask;
        bool result = hashmap_set(memory, c.address, new_value);
        if (!result) {
          printf("Couldn't set.\n");
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  
  uint64_t total = 0;
  for (int i = 0; i < memory->capacity; i++) {
    if (memory->entries[i].taken) total += memory->entries[i].value;
  }
  hashmap_free(memory);
  free(commands);
  return total;
}

int part2(const char* filename) {
  
}

int day14() {
  printf("====== Day 14 ======\n");
  printf("Part 1: %llu\n", part1("data/day14.txt"));
  printf("Part 2: %d\n", part2("data/day14.txt"));
  return EXIT_SUCCESS;
}
