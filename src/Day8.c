#include "Day8.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "parsing.h"

/// Day 8: Handheld Halting
/// 
/// Figure out how to break a handheld game out of an infinite loop!

/// The type of operation that is used
typedef enum {
  OP_NOP,  ///< No operation
  OP_ACC,  ///< Adjust acc by arg
  OP_JMP,  ///< Jump to arg location, relative to current ip
} Opcode;

/// A full instruction line in the program
typedef struct {
  Opcode op;  ///< The operation to be done
  int arg;    ///< A positive or negative integer argument
} Instruction;

/// A program that keeps track of whether it has already run code
typedef struct {
  Instruction* program;   ///< List of instructions to be run
  int ip;                 ///< Index of the current instruction to run
  int acc;                ///< Acc value adjusted by the acc opcode
  int lines;              ///< Number of lines in the program
  bool* line_run;         ///< List of booleans denoting whether each line has run yet
} Interpreter;

/// Check if the current line has already been run
#define HAS_ALREADY_RUN(interp) (interp.line_run[interp.ip])

/// Label this line as already run
#define MARK_AS_RUN(interp) (interp.line_run[interp.ip] = true)

/// Create a new interpreter from an input file
static Interpreter new_interpreter(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open input file.\n");
    exit(EXIT_FAILURE);
  }

  Interpreter interpreter;
  int lines = count_lines(fp);
  Instruction* program = (Instruction*)malloc(sizeof(Instruction) * lines);

  for (int i = 0; i < lines; i++) {
    char op[4] = {0};
    int arg = 0;
    fscanf(fp, "%s %d\n", op, &arg);
    Instruction instruction = {0};
    switch (op[0]) {
      case 'n':
        instruction.op = OP_NOP;
        break;
      case 'a':
        instruction.op = OP_ACC;
        break;
      case 'j':
        instruction.op = OP_JMP;
        break;
      default:
        printf("Unrecognized opcode %s.\n", op);
        exit(EXIT_FAILURE);
    }
    instruction.arg = arg;
    program[i] = instruction;
  }

  fclose(fp);
  interpreter.ip = 0;
  interpreter.acc = 0;
  interpreter.lines = lines;
  interpreter.line_run = (bool*)malloc(sizeof(bool) * lines);
  memset(interpreter.line_run, 0, sizeof(bool) * lines);
  interpreter.program = program;
  return interpreter;
}

/// Free an interpreter's malloced memory
void free_interpreter(Interpreter* interpreter) {
  free(interpreter->program);
  interpreter->program = NULL;
  free(interpreter->line_run);
  interpreter->line_run = NULL;
}

/// Run the instruction under the IP
void tick(Interpreter* interpreter) {
  Instruction instruction = interpreter->program[interpreter->ip];
  switch (instruction.op) {
      case OP_NOP: interpreter->ip++; break;
      case OP_ACC: 
        interpreter->acc += instruction.arg;
        interpreter->ip++;
        break;
      case OP_JMP: interpreter->ip += instruction.arg; break;
    }
}

/// What is the value of the acc when the first loop is encountered?
int part1(const char* filename) {
  Interpreter interpreter = new_interpreter(filename);

  while (true) {
    if (HAS_ALREADY_RUN(interpreter)) {
      return interpreter.acc;
    } else {
      MARK_AS_RUN(interpreter);
    }

    tick(&interpreter);
  }
}

/// Makes a standalone copy of an interpreter
Interpreter copy_interpreter(Interpreter* orig) {
  Interpreter new;
  new.ip = orig->ip;
  new.acc = orig->acc;
  new.lines = orig->lines;
  new.line_run = (bool*)malloc(sizeof(bool) * new.lines);
  memcpy(new.line_run, orig->line_run, sizeof(bool) * new.lines);
  new.program = (Instruction*)malloc(sizeof(Instruction) * new.lines);
  memcpy(new.program, orig->program, sizeof(Instruction) * new.lines);
  return new;
}

/// If you change one of the JMP to a NOP or vice versa, the program
/// will exit normally.  When it actually does that, what is the
/// value in acc?
int part2(const char* filename) {
  Interpreter base = new_interpreter(filename);

  for (int i = 0; i < base.lines; i++) {
    if (base.program[i].op == OP_ACC) continue;
    Interpreter interpreter = copy_interpreter(&base);
    if (interpreter.program[i].op == OP_JMP) {
      interpreter.program[i].op = OP_NOP;
    } else {
      interpreter.program[i].op = OP_JMP;
    }

    while (true) {
      if (HAS_ALREADY_RUN(interpreter)) {
        break;
      } else if (interpreter.ip >= interpreter.lines) {
        return interpreter.acc;
      } else {
        MARK_AS_RUN(interpreter);
      }

      tick(&interpreter);
    }

    free_interpreter(&interpreter);
  }
  return -1;
}

/// Run both days.
int day8() {
  printf("====== Day 8 ======\n");
  printf("Part 1: %d\n", part1("data/day8.txt"));
  printf("Part 2: %d\n", part2("data/day8.txt"));
  return EXIT_SUCCESS;
}
