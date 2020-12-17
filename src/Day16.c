#include "Day16.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "parsing.h"

typedef struct {
  char name[25];
  int min1;
  int max1;
  int min2;
  int max2;
} Field;

/// A field line is of the pattern:
/// <name>: <min1>-<max1> or <min2>-<max2>
Field parse_field_line(char* line) {
  Field f = {{0}, 0, 0, 0, 0};
  char* name = strtok(line, ":");
  char* min1 = strtok(NULL, "-");
  char* max1 = strtok(NULL, " ");
  strtok(NULL, " ");
  char* min2 = strtok(NULL, "-");
  char* max2 = strtok(NULL, "\n");
  strcpy(f.name, name);
  f.min1 = atoi(min1);
  f.max1 = atoi(max1);
  f.min2 = atoi(min2);
  f.max2 = atoi(max2);
  return f;
}

Field* parse_fields(FILE* fp, int* count) {
  *count = count_lines_until_blank(fp);
  Field* fields = (Field*)malloc(sizeof(Field) * *count);

  char line[100];
  for (int i = 0; i < *count; i++) {
    memset(line, 0, 100);
    fgets(line, 100, fp);
    fields[i] = parse_field_line(line);
  }
  return fields;
}

int* parse_my_values(FILE* fp, int field_count) {
  int* my_values = (int*)malloc(sizeof(int) * field_count);
  for (int i = 0; i < field_count; i++) {
    fscanf(fp, "%d", &my_values[i]);
    getc(fp);
  }
  return my_values;
}

int** parse_tickets(FILE* fp, int field_count, int* row_count) {
  size_t pos = ftell(fp);
  *row_count = count_lines(fp);
  fseek(fp, pos, SEEK_SET);

  int** rows = (int**)malloc(sizeof(int*) * *row_count);
  for (int i = 0; i < *row_count; i++) {
    rows[i] = (int*)malloc(sizeof(int) * field_count);
    for (int j = 0; j < field_count; j++) {
      fscanf(fp, "%d", &rows[i][j]);
      getc(fp);
    }
  }
  return rows;
}

/// Decides if a value is valid in at least one field.
static bool is_valid(int value, Field* fields, int field_count) {
  for (int i = 0; i < field_count; i++) {
    Field f = fields[i];
    if ((f.min1 <= value && f.max1 >= value) || (f.min2 <= value && f.max2 >= value)) {
      return true;
    }
  }
  return false;
}

/// Shifts the file pointer cursor forward n lines.
void consume_lines(FILE* fp, int n) {
  int newlines = 0;
  char c;
  while (newlines < n) {
    c = getc(fp);
    if (c == '\n') {
      newlines++;
    }
  }
}

/// Add up all of the invalid numbers from the nearby tickets.
int part1(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file.\n");
    exit(EXIT_FAILURE);
  }

  // Parse the file
  int field_count;
  Field* fields = parse_fields(fp, &field_count);
  consume_lines(fp, 5);
  int row_count;
  int** values = parse_tickets(fp, field_count, &row_count);

  // Add up all of the invalid values on each nearby ticket
  int total = 0;
  for (int i = 0; i < row_count; i++) {
    for (int j = 0; j < field_count; j++) {
      if (!is_valid(values[i][j], fields, field_count)) {
        total += values[i][j];
      }
    }
  }
      
  // Clean up and return
  fclose(fp);
  free(fields);
  for (int i = 0; i < row_count; i++) free(values[i]);
  free(values);

  return total;
}

/// Returns true if a ticket contains any invalid numbers
bool contains_invalid(int* numbers, Field* fields, int field_count) {
  for (int i = 0; i < field_count; i++) {
    if (!is_valid(numbers[i], fields, field_count)) return true;
  }
  return false;
}

void drop_invalid_tickets(int** tickets, int row_count, Field* fields, int field_count) {
  // Drop any nearby tickets with any invalid numbers
  for (int i = 0; i < row_count; i++) {
    if (contains_invalid(tickets[i], fields, field_count)) {
      free(tickets[i]);
      tickets[i] = NULL;
    }
  }
}

/// Creates a boolean grid where the rows represent fields and the
/// columns represent columns on a ticket.  A `true` represents that
/// that column could still possibly be represented by that field's rules
/// and all tickets so far have adhered to that field's rules in that
/// column.
bool** create_possibility_grid(int field_count) {
  bool** possibilities = (bool**)malloc(sizeof(bool*) * field_count);
    for (int i = 0; i < field_count; i++) {
      possibilities[i] = (bool*)malloc(sizeof(bool) * field_count);
      for (int j = 0; j < field_count; j++) {
        possibilities[i][j] = true;
      }
    }
  return possibilities;
}

/// Prune down the possibility values.  If a ticket has a value in
/// a column that breaks a rule, that rule can't apply to that column
/// for any ticket.
void rule_out_possibilities(bool** possibilities,
  int** tickets, int row_count, Field* fields, int field_count) {
  
  for (int i = 0; i < row_count; i++) {
    if (tickets[i] == NULL) continue;
    for (int j = 0; j < field_count; j++) {
      for (int k = 0; k < field_count; k++) {
        Field f = fields[k];
        int val = tickets[i][j];
        possibilities[k][j] &= ((f.min1 <= val && f.max1 >= val) ||
                                (f.min2 <= val && f.max2 >= val));
      }
    }
  }
}

/// Count the number of `true` cells in a row.
int count_possibles(bool** possibilities, int row, int field_count) {
  int count = 0;
  for (int i = 0; i < field_count; i++) {
    if (possibilities[row][i]) count++;
  }
  return count;
}

/// Find the one (and there will be only one) column index where the
/// cell in that row is `true`.
int find_possible(bool** possibilities, int row, int field_count) {
  for (int i = 0; i < field_count; i++) {
    if (possibilities[row][i]) return i;
  }
  return -1;
}

/// Find the one row (and there will always be exactly one) that
/// only has one column still true.  Assign that column to that field
/// and remove that as a possibility from all other fields.
void assign_decided_field(int* assignments, int field_count, bool** possibilities) {
  for (int row = 0; row < field_count; row++) {
    if (count_possibles(possibilities, row, field_count) != 1) {
      continue;
    }
    
    int col = find_possible(possibilities, row, field_count);
    assignments[row] = col;

    // Set all other rows to false for this column
    for (int k = 0; k < field_count; k++) {
      possibilities[k][col] = false;
    }
    
    return;  
  }
  printf("Couldn't find a decided row.\n");
  exit(EXIT_FAILURE);
}

/// Discover which fields go to which columns on the ticket by process
/// of elimination.  Multiply any values on my own ticket together
/// if the associated field starts with 'departure'
long part2(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file.\n");
    exit(EXIT_FAILURE);
  }

  // Parse file
  int field_count;
  Field* fields = parse_fields(fp, &field_count);
  consume_lines(fp, 2);
  int* my_values = parse_my_values(fp, field_count);
  consume_lines(fp, 2);
  int row_count;
  int** tickets = parse_tickets(fp, field_count, &row_count);

  // Preprocess tickets and possibilities
  drop_invalid_tickets(tickets, row_count, fields, field_count);
  
  bool** possible_fields = create_possibility_grid(field_count);
  rule_out_possibilities(possible_fields, tickets, row_count, fields, field_count);

  // Narrow down and assign columns by process of elimination.
  // Assumes there will always be exactly one row each iteration
  // With only one possible assignment.
  int assignments[field_count];
  for (int i = 0; i < field_count; i++) {
    assign_decided_field(assignments, field_count, possible_fields);
  }
  
  // Product up the departure columns
  long total = 1;
  for (int i = 0; i < field_count; i++) {
    if (strncmp(fields[i].name, "departure", 9) == 0) {
      total *= my_values[assignments[i]];
    }
  }

  // Clean up and return.
  fclose(fp);
  free(fields);
  for (int i = 0; i < field_count; i++) free(possible_fields[i]);
  free(possible_fields);
  for (int i = 0; i < row_count; i++) free(tickets[i]);
  free(tickets);
  free(my_values);

  return total;
}

int day16() {
  printf("====== Day 16 ======\n");
  printf("Part 1: %d\n", part1("data/day16.txt"));
  printf("Part 2: %ld\n", part2("data/day16.txt"));
  return EXIT_SUCCESS;
}
