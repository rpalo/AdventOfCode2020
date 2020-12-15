#ifndef AOC2020_HASHMAP_H
#define AOC2020_HASHMAP_H

/// A simple implementation of a hashmap using linear probing

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  unsigned int key;
  unsigned int value;
  bool taken;
} Entry;

typedef struct {
  Entry* entries;
  size_t capacity;
  size_t count;
} HashMap;

HashMap* hashmap_create(void);
void hashmap_free(HashMap* h);
Entry* find_entry(Entry* entries, size_t capacity, unsigned int key);
bool hashmap_expand(HashMap* h);
bool hashmap_set(HashMap* h, unsigned int key, unsigned int value);
unsigned int* hashmap_get(HashMap* h, unsigned int key);

#endif