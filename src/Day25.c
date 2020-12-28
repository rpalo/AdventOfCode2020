#include "Day25.h"

/// Day 25: Combo Breaker
///
/// "Break" into your hotel room by brute-forcing the shared key between
/// key card and door electronics.

#include <stdio.h>

#define PUB_KEY_SUBJECT 7 ///< The number used to generate public keys

/// Transfor a number (subject) based on a number of loops through a
/// simple hashing algorithm.
static int transform(long subject, long loop_size) {
  long long value = 1;
  for (long i = 0; i < loop_size; i++) {
    value *= subject;
    value %= 20201227;
  }
  return value;
}

/// A handshake consists of public keys from door and card that,
/// when looped by the other entity, come out to the same number.
typedef struct {
  long door_key;
  long card_key;
  long door_loop_size;
  long card_loop_size;
} RFIDHandshake;

/// Parse the input file, which is just two lines with one integer
/// per line.
static RFIDHandshake parse(const char* filename) {
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Couldn't open input file.\n");
    exit(EXIT_FAILURE);
  }
  RFIDHandshake key;
  fscanf(fp, "%ld\n%ld", &key.card_key, &key.door_key);
  fclose(fp);
  return key;
}

/// Get the loop size from a public key by guessing and checking each
/// loop size until it matches.
static long loop_size(long pub_key) {
  for (long loop_size = 0; loop_size < 1000000; loop_size++) {
    if (transform(PUB_KEY_SUBJECT, loop_size) == pub_key) {
      return loop_size;
    }
  }
  return -1;
}

/// Figure out what the shared final key is between door and card.
long part1(const char* filename) {
  RFIDHandshake keys = parse(filename);
  keys.card_loop_size = loop_size(keys.card_key);
  if (keys.door_loop_size == -1) {
    printf("Couldn't get door loop size.\n");
    exit(EXIT_FAILURE);
  }
  return transform(keys.door_key, keys.card_loop_size);
}

int day25() {
  printf("====== Day 25 ======\n");
  printf("Part 1: %ld\n", part1("data/day25.txt"));
  return EXIT_SUCCESS;
}
