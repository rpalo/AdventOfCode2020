/// Day 2: Password Philosophy 
/// 
/// Find the passwords that aren't compliant.

#include <stdlib.h>
#include <stdint.h>

/// A Policied Password is a password that is accompanied by a policy
/// consisting of two positive integers and a letter.  These components
/// can be used to validate the password.
typedef struct {
  int a;
  int b;
  char letter;
  char password[30];
} PoliciedPassword;

/// Part 1 calculates how many valid passwords there are.
/// A password is valid if the # of occurrences of 'letter' is between
/// 'a' and 'b,' inclusive.
///
/// passes: the list of policied passwords to check
/// count: the number of passwords to check
int part1(PoliciedPassword** passes, size_t count);

/// Part 2 calculates how many valid passwords there are.
/// A password is valid of either the 'a' index or 'b' index character 
/// (1-based!) is equal to 'letter,' but not both.
int part2(PoliciedPassword** passes, size_t count);

/// day2 runs both parts in sequence and outputs their results.
int day2();