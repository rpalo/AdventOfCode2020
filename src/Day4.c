#include "Day4.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Loops through a file and counts all instances of double-newlines.
/// Adds one at the end for the last entry which doesn't have a trailing
/// double-newline.
static int count_passports(FILE* fp) {
  int count = 0;
  while (!feof(fp)) {
    if (getc(fp) == '\n' && getc(fp) == '\n') count++;
  }
  count++; // No double newline after last one.
  
  rewind(fp);
  return count;
}

/// Parses a single passport from a file.  Leaves the file pointer 
/// after the trailing double-newline, ready to parse the next one.
static SimplePassport parse_simple_passport(FILE* fp) {
  char key[4];
  char value[20];
  SimplePassport passport = {0};

  while (!feof(fp) && getc(fp) != '\n') {
    memset(key, 0, 4);
    memset(value, 0, 20);
    fseek(fp, -1, SEEK_CUR); // Un-eat the previous char because we need it.
    fgets(key, KEY_SIZE, fp);

    fscanf(fp, "%s", value);  // Eat the colon, value
    getc(fp);  // Eat white space?

    if (strcmp(key, "byr") == 0) passport.byr = true;
    else if (strcmp(key, "iyr") == 0) passport.iyr = true;
    else if (strcmp(key, "eyr") == 0) passport.eyr = true;
    else if (strcmp(key, "hgt") == 0) passport.hgt = true;
    else if (strcmp(key, "hcl") == 0) passport.hcl = true;
    else if (strcmp(key, "ecl") == 0) passport.ecl = true;
    else if (strcmp(key, "pid") == 0) passport.pid = true;
    else if (strcmp(key, "cid") == 0) passport.cid = true;
    else {
      printf("Unrecognized key: %s\n", key);
    }
  }
  return passport;
}

SimplePassport* parse_simple_passports(const char* filename, int* count) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  *count = count_passports(fp);
  SimplePassport* passes = (SimplePassport*)malloc(sizeof(SimplePassport) * *count);

  for (int i = 0; i < *count; i++) {
    passes[i] = parse_simple_passport(fp);
  }

  fclose(fp);
  return passes;
}

int part1(SimplePassport* passes, int count) {
  int invalid = 0;
  for (int i = 0; i < count; i++) {
    bool* p = (bool*)&passes[i];

    // Iterates over each field of the passport by using the bytes of
    // the struct as an array of bools.  Which should be OK?
    for (int j = 0; j < NUM_PASS_FIELDS - 1; j++) {
      if (p[j] == false) {
        invalid++;
        break;
      };
    }
  }
  return count - invalid;
}

// ================= Part 2 ===================== //

/// Parses a height string to a Height struct.  Well-formed strings
/// are [0-9]+(in|cm).  If it's invalid, leaves the value as 0 and
/// the units as HT_BAD.
static Height parse_height(char* value) {
  Height h;
  int possible_value;
  char units[4];
  sscanf(value, "%d%s", &possible_value, units);

  if (strcmp(units, "in") == 0) {
    h.value = possible_value;
    h.units = HT_IN;
  } else if (strcmp(units, "cm") == 0) {
    h.value = possible_value;
    h.units = HT_CM;
  } else {
    h.value = 0;
    h.units = HT_BAD;
  }
  return h;
}

/// Parses an EyeColor from a string.
static EyeColor parse_eye_color(char* value) {
  if (strcmp(value, "amb") == 0) return EYE_AMBER;
  if (strcmp(value, "blu") == 0) return EYE_BLUE;
  if (strcmp(value, "brn") == 0) return EYE_BROWN;
  if (strcmp(value, "gry") == 0) return EYE_GREY;
  if (strcmp(value, "grn") == 0) return EYE_GREEN;
  if (strcmp(value, "hzl") == 0) return EYE_HAZEL;
  if (strcmp(value, "oth") == 0) return EYE_OTHER;
  return EYE_BAD;
}

/// Prints out a FancyPassport for debugging.
static inline void print_fancy_passport(FancyPassport* p) {
  printf("Passport:\n");
  printf("BYR: %d\nIYR: %d\nEYR: %d\n", p->byr, p->iyr, p->eyr);
  printf("HGT: %d-%d\nHCL: %s\nECL: %d\n", p->hgt.value, p->hgt.units, p->hcl, p->ecl);
  printf("PID: %s\n", p->pid);
}

/// Parses a single fancy passport from a file.
static FancyPassport parse_fancy_passport(FILE* fp) {
  char key[KEY_SIZE];
  char value[20];
  FancyPassport passport = {0};

  while (!feof(fp) && getc(fp) != '\n') {
    memset(key, 0, 4);
    memset(value, 0, 20);
    fseek(fp, -1, SEEK_CUR); // Un-eat the previous char because we need it.
    fgets(key, KEY_SIZE, fp);

    fscanf(fp, ":%s", value);  // Eat the colon, value
    getc(fp);  // Eat white space

    if (strcmp(key, "byr") == 0) passport.byr = atoi(value);
    else if (strcmp(key, "iyr") == 0) passport.iyr = atoi(value);
    else if (strcmp(key, "eyr") == 0) passport.eyr = atoi(value);
    else if (strcmp(key, "hgt") == 0) passport.hgt = parse_height(value);
    else if (strcmp(key, "hcl") == 0) {
      if (strlen(value) != HAIR_COLOR_SIZE - 1) continue;  // Leave it empty
      strncpy(passport.hcl, value, HAIR_COLOR_SIZE - 1);
    }
    else if (strcmp(key, "ecl") == 0) passport.ecl = parse_eye_color(value);
    else if (strcmp(key, "pid") == 0) {
      if (strlen(value) != PID_SIZE - 1) continue;  // Leave it empty
      strncpy(passport.pid, value, PID_SIZE - 1);
    } 
    else if (strcmp(key, "cid") == 0) continue;
    else {
      printf("Unrecognized key: %s\n", key);
    }
  }

  return passport;
}

FancyPassport* parse_fancy_passports(const char* filename, int* count) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open file.\n");
    exit(EXIT_FAILURE);
  }

  *count = count_passports(fp);
  FancyPassport* passes = (FancyPassport*)malloc(sizeof(FancyPassport) * *count);

  for (int i = 0; i < *count; i++) {
    passes[i] = parse_fancy_passport(fp);
  }

  fclose(fp);
  return passes;
}

/// Checks whether a hair color is valid.
static bool valid_hair_color(char* color) {
  if (color[0] != '#') return false;
  for (int i = 1; color[i]; i++) {
    if ((color[i] < '0' || color[i] > '9') && (color[i] < 'a' || color[i] > 'f')) {
      return false;
    }
  }
  return true;
}

/// Checks whether a PID is valid.
static bool valid_pid(char* value) {
  if (strlen(value) != PID_SIZE - 1) return false;
  for (int i = 0; value[i]; i++) {
    if (value[i] < '0' || value[i] > '9') return false;
  }
  return true;
}

/// Finger-saving macro for part2: increment the invalids and continue
#define INC_BAD {invalid++; continue;}

int part2(FancyPassport* passes, int count) {
  int invalid = 0;
  for (int i = 0; i < count; i++) {
    FancyPassport p = passes[i];
    
    if (p.byr < 1920 || p.byr > 2002) INC_BAD
    if (p.iyr < 2010 || p.iyr > 2020) INC_BAD
    if (p.eyr < 2020 || p.eyr > 2030) INC_BAD
    if (p.hgt.units == HT_BAD) INC_BAD
    if (p.hgt.units == HT_CM && (p.hgt.value < 150 || p.hgt.value > 193)) INC_BAD
    if (p.hgt.units == HT_IN && (p.hgt.value < 59 || p.hgt.value > 76)) INC_BAD
    if (!valid_hair_color(p.hcl)) INC_BAD
    if (p.ecl == EYE_BAD) INC_BAD
    if (!valid_pid(p.pid)) INC_BAD
  }
  return count - invalid;
}

int day4() {
  int count;
  SimplePassport* passes = parse_simple_passports("data/day4.txt", &count);
  printf("====== Day 4 ======\n");
  printf("Part 1: %d\n", part1(passes, count));
  free(passes);

  count = 0;
  FancyPassport* passes2 = parse_fancy_passports("data/day4.txt", &count);
  printf("Part 2: %d\n", part2(passes2, count));
  free(passes2);
  return EXIT_SUCCESS;
}
