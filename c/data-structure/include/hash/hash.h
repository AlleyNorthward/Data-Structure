#ifndef HASH_H
#define HASH_H

#include "array/DArray.h"
#include "dsdef.h"
#include "list/List.h"

typedef struct HashNode {
  Node link;
  void *payload;
} HashNode;

#define hash_node(ptr) container_of(ptr, HashNode, link)

typedef struct HashTable {
  DArray *buckets;
  int bucket_count;
  int total_elements;
  float max_load_factor;

  void *(*extract_key)(void *elem);
  unsigned int (*hash)(void *key);
  bool (*equal)(void *key_a, void *key_b);
  void (*free_payload)(void *elem);
} HashTable;

HashTable *newHashTable(int bucket_count, void *(*extract_key)(void *elem),
                        unsigned int (*hash)(void *key),
                        bool (*equal)(void *key_a, void *key_b),
                        void (*free_payload)(void *elem));

void destroy_H(HashTable *ht);
void clear_H(HashTable *ht);
bool empty_H(HashTable *ht);

int size_H(HashTable *ht);
void *find_H(HashTable *ht, void *key);

bool insert_unique_H(HashTable *ht, void *elem);
void insert_equal_H(HashTable *ht, void *elem);
int erase_H(HashTable *ht, void *key);

void rehash_H(HashTable *ht, int new_bucket_count);

#endif
