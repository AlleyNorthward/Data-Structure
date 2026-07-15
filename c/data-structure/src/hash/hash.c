#include "hash/hash.h"
#include <stdlib.h>

static void free_L(Node *node, void *args) {
  HashNode *ht = hash_node(node);
  if (args) {
    void (*free_value)(void *elem) = (void (*)(void *))args;
    free_value(ht->payload);
  }
  free(ht);
}

static void free_DA(void *elem, void *args) {
  List *list = *(List **)elem;
  destroy_L(list, free_L, args);
}

static inline int get_bucket_index(HashTable *ht, void *key) {
  unsigned int hash_val = ht->hash(key);
  return hash_val % ht->bucket_count;
}

static HashNode *find(HashTable *ht, void *key, int *out_index) {
  int index = get_bucket_index(ht, key);
  if (out_index)
    *out_index = index;

  List *bucket = GET_DA(ht->buckets, index, List *);
  if (bucket == NULL)
    return NULL;

  Node *cur = front_L(bucket);
  while (cur) {
    HashNode *hashnode = hash_node(cur);
    void *existing_key = ht->extract_key(hashnode->payload);
    if (ht->equal(key, existing_key))
      return hashnode;
    cur = cur->next;
  }
  return NULL;
}

static void rehash(HashTable *ht, int new_bucket_count) {
  if (ht == NULL || new_bucket_count <= 0 ||
      new_bucket_count == ht->bucket_count)
    return;

  DArray *old_buckets = ht->buckets;
  int old_bucket_count = ht->bucket_count;

  ht->bucket_count = new_bucket_count;
  ht->buckets = newDArray(new_bucket_count, sizeof(List *));
  if (!ht->buckets) {
    ht->buckets = old_buckets;
    ht->bucket_count = old_bucket_count;
    return;
  }

  for (int i = 0; i < new_bucket_count; ++i) {
    SET_DA(ht->buckets, i, NULL, List *);
  }

  for (int i = 0; i < old_bucket_count; ++i) {
    List *old_list = GET_DA(old_buckets, i, List *);
    if (old_list == NULL || empty_L(old_list))
      continue;

    while (!empty_L(old_list)) {
      Node *node = pop_front_L(old_list);
      if (node == NULL) {
        break;
      }
      HashNode *hn = hash_node(node);
      void *key = ht->extract_key(hn->payload);
      int new_index = get_bucket_index(ht, key);

      List *new_list = GET_DA(ht->buckets, new_index, List *);
      if (new_list == NULL) {
        new_list = newList();
        if (new_list == NULL) {
          ht->free_payload(hn->payload);
          free(hn);

          destroy_DA(old_buckets, free_DA, ht->free_payload);
          destroy_DA(ht->buckets, free_DA, ht->free_payload);

          ht->buckets = NULL;
          ht->total_elements = 0;
          return;
        }
        SET_DA(ht->buckets, new_index, new_list, List *);
      }
      push_front_L(new_list, node);
    }
  }
  destroy_DA(old_buckets, free_DA, NULL);
}

static bool insert(HashTable *ht, void *elem, bool is_unique) {
  if (ht == NULL || elem == NULL)
    return false;

  void *key = ht->extract_key(elem);
  int index = get_bucket_index(ht, key);

  if (is_unique && find(ht, key, &index)) {
    return false;
  }

  HashNode *new_node = malloc(sizeof(HashNode));
  if (new_node == NULL)
    return false;
  new_node->payload = elem;

  List *bucket = GET_DA(ht->buckets, index, List *);
  if (bucket == NULL) {
    bucket = newList();
    if (bucket == NULL) {
      free(new_node);
      return false;
    }
    SET_DA(ht->buckets, index, bucket, List *);
  }
  push_front_L(bucket, &new_node->link);
  ++ht->total_elements;

  if (ht->total_elements > ht->max_load_factor * ht->bucket_count) {
    rehash(ht, ht->bucket_count * 2);
  }
  return true;
}

HashTable *newHashTable(int bucket_count, void *(*extract_key)(void *elem),
                        unsigned int (*hash)(void *key),
                        bool (*equal)(void *key_a, void *key_b),
                        void (*free_payload)(void *elem)) {
  if (bucket_count <= 0)
    bucket_count = 53;
  if (extract_key == NULL || hash == NULL || equal == NULL)
    return NULL;

  HashTable *ht = malloc(sizeof(HashTable));
  if (ht == NULL)
    return NULL;

  ht->bucket_count = bucket_count;
  ht->total_elements = 0;
  ht->max_load_factor = 1.0f;
  ht->extract_key = extract_key;
  ht->hash = hash;
  ht->equal = equal;
  ht->free_payload = free_payload;

  ht->buckets = newDArray(bucket_count, sizeof(List *));
  if (ht->buckets == NULL) {
    free(ht);
    return NULL;
  }

  for (int i = 0; i < bucket_count; ++i) {
    List *list = newList();
    if (list == NULL) {
      destroy_DA(ht->buckets, free_DA, NULL);
      return NULL;
    }
    SET_DA(ht->buckets, i, list, List *);
  }
  return ht;
}

void destroy_H(HashTable *ht) {
  if (ht == NULL)
    return;

  destroy_DA(ht->buckets, free_DA, ht->free_payload);
  free(ht);
}

void clear_H(HashTable *ht) {
  if (ht == NULL || ht->bucket_count == 0)
    return;

  for (int i = 0; i < ht->bucket_count; ++i) {
    List *bucket = GET_DA(ht->buckets, i, List *);
    clear_L(bucket, free_L, ht->free_payload);
  }
  ht->total_elements = 0;
}

bool empty_H(HashTable *ht) {
  if (ht == NULL)
    return true;
  return ht->total_elements == 0;
}

int size_H(HashTable *ht) {
  if (ht == NULL)
    return 0;
  return ht->total_elements;
}
void *find_H(HashTable *ht, void *key) {
  if (ht == NULL || key == NULL)
    return NULL;
  HashNode *hn = find(ht, key, NULL);
  if (hn == NULL)
    return NULL;
  return hn->payload;
}

bool insert_unique_H(HashTable *ht, void *elem) {
  return insert(ht, elem, true);
}

void insert_equal_H(HashTable *ht, void *elem) { insert(ht, elem, false); }

int erase_H(HashTable *ht, void *key) {
  if (ht == NULL || key == NULL)
    return 0;

  int index = get_bucket_index(ht, key);
  List *bucket = GET_DA(ht->buckets, index, List *);
  if (bucket == NULL)
    return 0;

  Node *pre = NULL;
  Node *next = NULL;
  Node *cur = front_L(bucket);
  int erased = 0;

  while (cur) {
    HashNode *hn = hash_node(cur);
    void *existint_key = ht->extract_key(hn->payload);
    if (ht->equal(key, existint_key)) {
      next = cur->next;
      Node *node = erase_L(bucket, pre);
      if (node == NULL)
        return 0;
      HashNode *del = hash_node(node);

      if (ht->free_payload) {
        ht->free_payload(hn->payload);
      }

      free(del);

      --ht->total_elements;
      ++erased;
      cur = next;
    } else {
      pre = cur;
      cur = cur->next;
    }
  }
  return erased;
}

void rehash_H(HashTable *ht, int new_bucket_count) {
  rehash(ht, new_bucket_count);
}
