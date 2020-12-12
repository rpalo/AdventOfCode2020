#include "Day12.h"

#include <stdio.h>

#include "parsing.h"

/// Direction enum for part 1
typedef enum {
  DIR_NORTH,
  DIR_EAST,
  DIR_SOUTH,
  DIR_WEST,
} Direction;

/// An instruction is one command to move the ship or waypoint
typedef struct {
  Action action;  ///< The `Action` to perform
  int amount;     ///< A measure of how much to perform that action
} Instruction;

/// Parse the input file, which has lines of 'A123' where 'A' is a 
/// character specifying the action to take and '123' is some 1-3 digit
/// number that specifies the "amount" to do the action.
static Instruction* parse(const char* filename, int* count) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  *count = count_lines(fp);
  Instruction* instructions = (Instruction*)malloc(sizeof(Instruction) * *count);

  for (int i = 0; i < *count; i++) {
    char c = 0;
    int arg = 0;
    fscanf(fp, "%c%d\n", &c, &arg);
    Action action;
    switch (c) {
      case 'N': action = AC_NORTH; break;
      case 'E': action = AC_EAST; break;
      case 'S': action = AC_SOUTH; break;
      case 'W': action = AC_WEST; break;
      case 'L': action = AC_LEFT; break;
      case 'R': action = AC_RIGHT; break;
      case 'F': action = AC_FORWARD; break;
      default: printf("Unrecognized action: %c\n", c); exit(EXIT_FAILURE);
    }
    instructions[i] = (Instruction){.action = action, .amount = arg};
  }

  fclose(fp);
  return instructions;
}

/// Calculate the new direction based on an orientation and an amount
/// to turn.
static Direction turn(Direction current, Action action, int amount) {
  int rotation = action == AC_LEFT ? -1 : 1;
  int new_dir = ((int)current + (amount / 90) * rotation) % 4;
  if (new_dir < 0) new_dir += 4;
  return (Direction)new_dir;
}

int part1(const char* filename) {
  int count;
  Instruction* instructions = parse(filename, &count);

  int x = 0;
  int y = 0;
  Direction direction = DIR_EAST;

  for (int i = 0; i < count; i++) {
    Instruction inst = instructions[i];
    #ifdef STEPTHRU
    printf("<%d, %d> Facing %d.\n", x, y, direction);
    printf("%d: %d\n", inst.action, inst.amount);
    getc(stdin);
    #endif
    switch (inst.action) {
      case AC_NORTH: y += inst.amount; break;
      case AC_EAST: x += inst.amount; break;
      case AC_SOUTH: y -= inst.amount; break;
      case AC_WEST: x -= inst.amount; break;
      case AC_LEFT: direction = turn(direction, AC_LEFT, inst.amount); break;
      case AC_RIGHT: direction = turn(direction, AC_RIGHT, inst.amount); break;
      case AC_FORWARD: {
        switch (direction) {
          case DIR_NORTH: y += inst.amount; break;
          case DIR_EAST: x += inst.amount; break;
          case DIR_SOUTH: y -= inst.amount; break;
          case DIR_WEST: x -= inst.amount; break;
          default: 
            printf("Unrecognized direction state: %d\n", direction);
            exit(EXIT_FAILURE);
        };
        break;
      }
      default: printf("Unrecognized action. %d\n", inst.action); exit(EXIT_FAILURE);
    }
  }

  return abs(x) + abs(y);
}

ShipState move(ShipState current, Action action, int amount) {
  ShipState new = current;
  switch (action) {
    case AC_NORTH: new.wy += amount; break;
    case AC_EAST: new.wx += amount; break;
    case AC_SOUTH: new.wy -= amount; break;
    case AC_WEST: new.wx -= amount; break;
    case AC_LEFT: {
      for (int j = 0; j < amount / 90; j++) {
        int tmp = new.wx;
        new.wx = new.wy*-1;
        new.wy = tmp;
      }
    }; break;
    case AC_RIGHT: {
      for (int j = 0; j < amount / 90; j++) {
        int tmp = new.wx;
        new.wx = new.wy;
        new.wy = tmp*-1;
      }
    }; break;
    case AC_FORWARD: {
      new.x += new.wx * amount;
      new.y += new.wy * amount;
    }; break;
    default: printf("Unrecognized action. %d\n", action); exit(EXIT_FAILURE);
  }
  return new;
}

int part2(const char* filename) {
  int count;
  Instruction* instructions = parse(filename, &count);

  ShipState state = {0, 0, 10, 1};

  for (int i = 0; i < count; i++) {
    Instruction inst = instructions[i];
    #ifdef STEPTHRU
    printf("<%d, %d> wpt <%d, %d>.\n", state.x, state.y, state.wx, state.wy);
    printf("%s: %d\n", actions[inst.action], inst.amount);
    getc(stdin);
    #endif
    state = move(state, inst.action, inst.amount);
  }

  return abs(state.x) + abs(state.y);
}

int day12() {
  printf("====== Day 12 ======\n");
  printf("Part 1: %d\n", part1("data/day12.txt"));
  printf("Part 2: %d\n", part2("data/day12.txt"));
  return EXIT_SUCCESS;
}
