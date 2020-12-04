#ifndef AOC2020_DAY4_H
#define AOC2020_DAY4_H

/// Day 4: Passport Processing
///
/// Pick through passports made up of key/value pairs to figure out
/// which ones are valid.

#include <stdlib.h>
#include <stdbool.h>

#define NUM_PASS_FIELDS 8  ///< Number of fields in a SimplePassport
#define MAX_ENTRY_SIZE 20  ///< Max size of possible value in key/value
#define KEY_SIZE 4         ///< Size of a key in key/value
#define HAIR_COLOR_SIZE 8  ///< Size of a well-formed hair color
#define PID_SIZE 10        ///< Size of a well-formed PID

/// A simple passport just has certain fields or not.  Valid ones have
/// them all, with 'cid' being optional.
typedef struct {
  bool byr;
  bool iyr;
  bool eyr;
  bool hgt;
  bool hcl;
  bool ecl;
  bool pid;
  bool cid;
} SimplePassport;

/// Parses the input file, which is a series of passports.  Each key/val
/// is separated by a space or newline.  Passports are separated by 
/// two newlines.  Returns a list of passport structs.
SimplePassport* parse_simple_passports(const char* filename, int* count);

/// Counts the number of valid passports.  Passports are valid if
/// the have all keys, except CID is optional.
int part1(SimplePassport* passes, int count);

// ============== Part 2 ================== //

/// Possible values for height units: none, inches, or centimeters.
typedef enum {
  HT_BAD,
  HT_IN,
  HT_CM,
} HeightUnit;

/// A height is a measurement associated with a set of units.
typedef struct {
  int value;
  HeightUnit units;
} Height;

/// Possible options for eye color
typedef enum {
  EYE_BAD,
  EYE_AMBER,
  EYE_BLUE,
  EYE_BROWN,
  EYE_GREY,
  EYE_GREEN,
  EYE_HAZEL,
  EYE_OTHER,
} EyeColor;

/// A fancy passport has strict value validation for all fields.
/// Note: all fields here must be present to win.
typedef struct {
  /// Birth Year: 1920 <= x <= 2002
  int byr; 
  /// Issue Year: 2010 <= x <= 2020
  int iyr;
  /// Expiration Year: 2020 <= x <= 2030
  int eyr;
  /// Height: 150 <= cm <= 193 || 59 <= in <= 76
  Height hgt;
  /// Hair Color: # followed by 6 chars 0-9 or a-f
  char hcl[8];
  /// Eye Color: amb | blu | brn | gry | grn | hzl | oth
  EyeColor ecl;
  /// Passport ID : Exactly 9 digits.  (Storing 10 to find invalid ones)
  char pid[10];
} FancyPassport;

/// Parse fancy passports from an input file.  The number of them
/// is stored in 'count'.
FancyPassport* parse_fancy_passports(const char* filename, int* count);

/// Counts the number of valid fancy passports based on the rules above.
int part2(FancyPassport* passes, int count);

/// Runs both parts.
int day4(void);
#endif
