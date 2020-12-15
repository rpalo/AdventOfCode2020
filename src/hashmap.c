#include "hashmap.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_MEMORY_SIZE 8
#define MAX_LOAD 0.75

HashMap* hashmap_create(void) {
  HashMap* h = (HashMap*)malloc(sizeof(HashMap));
  h->entries = NULL;
  h->capacity = 0;
  h->count = 0;
  return h;
}

void hashmap_free(HashMap* h) {
  free(h->entries);
  free(h);
}

Entry* find_entry(Entry* entries, size_t capacity, unsigned int key) {
  size_t index = key % capacity;

  for (;;) {
    Entry* entry = &entries[index];

    if (entry->key == key || entry->taken == false) {
      return entry;
    }
    index = (index + 1) % capacity;
  }
}

bool hashmap_expand(HashMap* h) {
  size_t new_capacity = h->capacity == 0 ? 8 : h->capacity * 2;
  Entry* new_entries = (Entry*)malloc(sizeof(Entry) * new_capacity);
  if (new_entries == NULL) {
    return false;
  }

  for (size_t i = 0; i < new_capacity; i++) {
    new_entries[i] = (Entry){0, 0, false};
  }

  for (size_t i = 0; i < h->capacity; i++) {
    Entry* entry = &h->entries[i];
    if (!entry->taken) continue;
    Entry* dest = find_entry(new_entries, new_capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    dest->taken = true;
  }

  free(h->entries);
  h->entries = new_entries;
  h->capacity = new_capacity;
  return true;
}

bool hashmap_set(HashMap* h, unsigned int key, unsigned int value) {
  if (h->count + 1 > h->capacity * MAX_LOAD) {
    if (hashmap_expand(h) == false) return false;
  }

  Entry* entry = find_entry(h->entries, h->capacity, key);
  if (!entry->taken) {
    h->count++;
    entry->key = key;
    entry->taken = true;
  }
  entry->value = value;
  return true;
}
