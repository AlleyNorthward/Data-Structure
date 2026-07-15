#include "acutest.h"
#include "hash/hash.h"
#include <stdlib.h>

typedef struct {
  int key;
  int value;
} Payload;

static Payload *make_payload(int key, int value) {
  Payload *p = malloc(sizeof(Payload));
  p->key = key;
  p->value = value;
  return p;
}

static void *extract_key(void *elem) {
  Payload *p = (Payload *)elem;
  return &p->key;
}

static unsigned int hash_func(void *key) {
  int k = *(int *)key;
  return (unsigned int)(k * 2654435761u);
}

static bool equal_func(void *a, void *b) { return *(int *)a == *(int *)b; }

static void free_payload(void *elem) { free(elem); }

static bool contains_key(HashTable *ht, int key) {
  void *found = find_H(ht, &key);
  if (!found)
    return false;
  Payload *p = (Payload *)found;
  return p->key == key;
}

static int get_value(HashTable *ht, int key) {
  void *found = find_H(ht, &key);
  if (!found)
    return -1;
  return ((Payload *)found)->value;
}

static unsigned int hash_collision(void *key) {
  (void)key;
  return 0;
}

void test_hash_create_destroy(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  TEST_CHECK(empty_H(ht) == true);
  TEST_CHECK(size_H(ht) == 0);
  TEST_CHECK(ht->bucket_count == 16);
  destroy_H(ht);

  ht = newHashTable(0, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  TEST_CHECK(ht->bucket_count == 53);
  destroy_H(ht);
}

void test_hash_insert_unique_find(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  for (int i = 0; i < 100; ++i) {
    Payload *p = make_payload(i, i * 10);
    TEST_CHECK(insert_unique_H(ht, p) == true);
  }
  TEST_CHECK(size_H(ht) == 100);
  for (int i = 0; i < 100; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
    TEST_CHECK(get_value(ht, i) == i * 10);
  }
  TEST_CHECK(contains_key(ht, 200) == false);
  TEST_CHECK(find_H(ht, NULL) == NULL);
  destroy_H(ht);
}

void test_hash_insert_unique_duplicate(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  Payload *p1 = make_payload(5, 100);
  TEST_CHECK(insert_unique_H(ht, p1) == true);
  Payload *p2 = make_payload(5, 200);
  TEST_CHECK(insert_unique_H(ht, p2) == false);
  free(p2);
  TEST_CHECK(size_H(ht) == 1);
  TEST_CHECK(get_value(ht, 5) == 100);
  destroy_H(ht);
}

void test_hash_insert_equal(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  Payload *p1 = make_payload(3, 10);
  Payload *p2 = make_payload(3, 20);
  Payload *p3 = make_payload(3, 30);
  insert_equal_H(ht, p1);
  insert_equal_H(ht, p2);
  insert_equal_H(ht, p3);
  TEST_CHECK(size_H(ht) == 3);
  void *found = find_H(ht, &(int){3});
  TEST_CHECK(found != NULL);
  int val = ((Payload *)found)->value;
  TEST_CHECK(val == 10 || val == 20 || val == 30);
  int erased = erase_H(ht, &(int){3});
  TEST_CHECK(erased == 3);
  TEST_CHECK(size_H(ht) == 0);
  destroy_H(ht);
}

void test_hash_find_order_after_equal_insert(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  Payload *p1 = make_payload(1, 10);
  Payload *p2 = make_payload(1, 20);
  Payload *p3 = make_payload(1, 30);
  insert_equal_H(ht, p1);
  insert_equal_H(ht, p2);
  insert_equal_H(ht, p3);
  Payload *found = (Payload *)find_H(ht, &(int){1});
  TEST_CHECK(found->value == 30);
  int erased = erase_H(ht, &(int){1});
  TEST_CHECK(erased == 3);
  TEST_CHECK(size_H(ht) == 0);
  destroy_H(ht);
}

void test_hash_erase_single(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 10; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == 10);
  for (int i = 0; i < 10; ++i) {
    int erased = erase_H(ht, &i);
    TEST_CHECK(erased == 1);
    TEST_CHECK(size_H(ht) == 9 - i);
    TEST_CHECK(contains_key(ht, i) == false);
    for (int j = i + 1; j < 10; ++j) {
      TEST_CHECK(contains_key(ht, j) == true);
    }
  }
  TEST_CHECK(size_H(ht) == 0);
  destroy_H(ht);
}

void test_hash_erase_nonexistent(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 5; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(erase_H(ht, &(int){10}) == 0);
  TEST_CHECK(size_H(ht) == 5);
  destroy_H(ht);
}

void test_hash_erase_middle(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 10; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  erase_H(ht, &(int){3});
  TEST_CHECK(contains_key(ht, 2) == true);
  TEST_CHECK(contains_key(ht, 4) == true);
  erase_H(ht, &(int){7});
  TEST_CHECK(contains_key(ht, 6) == true);
  TEST_CHECK(contains_key(ht, 8) == true);
  TEST_CHECK(size_H(ht) == 8);
  destroy_H(ht);
}

void test_hash_erase_head_tail(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 10; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  erase_H(ht, &(int){0});
  TEST_CHECK(contains_key(ht, 0) == false);
  TEST_CHECK(contains_key(ht, 1) == true);
  erase_H(ht, &(int){9});
  TEST_CHECK(contains_key(ht, 9) == false);
  TEST_CHECK(contains_key(ht, 8) == true);
  TEST_CHECK(size_H(ht) == 8);
  destroy_H(ht);
}

void test_hash_clear(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 10; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == 10);
  clear_H(ht);
  TEST_CHECK(size_H(ht) == 0);
  TEST_CHECK(empty_H(ht) == true);
  Payload *p = make_payload(99, 999);
  insert_unique_H(ht, p);
  TEST_CHECK(size_H(ht) == 1);
  TEST_CHECK(contains_key(ht, 99) == true);
  clear_H(ht);
  TEST_CHECK(size_H(ht) == 0);
  destroy_H(ht);
}

void test_hash_load_factor(void) {
  HashTable *ht =
      newHashTable(4, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  TEST_CHECK(ht->max_load_factor == 1.0f);
  TEST_CHECK(ht->bucket_count == 4);
  for (int i = 0; i < 4; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(ht->bucket_count == 4);
  Payload *p5 = make_payload(4, 4);
  insert_unique_H(ht, p5);
  TEST_CHECK(ht->bucket_count == 8);
  TEST_CHECK(size_H(ht) == 5);
  destroy_H(ht);
}

void test_hash_rehash_auto(void) {
  HashTable *ht =
      newHashTable(4, extract_key, hash_func, equal_func, free_payload);
  int n = 100;
  for (int i = 0; i < n; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == n);
  for (int i = 0; i < n; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
    TEST_CHECK(get_value(ht, i) == i);
  }
  destroy_H(ht);
}

void test_hash_rehash_manual(void) {
  HashTable *ht =
      newHashTable(8, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 20; ++i) {
    Payload *p = make_payload(i, i * 2);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == 20);
  rehash_H(ht, 32);
  TEST_CHECK(ht->bucket_count == 32);
  TEST_CHECK(size_H(ht) == 20);
  for (int i = 0; i < 20; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
    TEST_CHECK(get_value(ht, i) == i * 2);
  }
  rehash_H(ht, 4);
  TEST_CHECK(ht->bucket_count == 4);
  TEST_CHECK(size_H(ht) == 20);
  for (int i = 0; i < 20; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  rehash_H(ht, 20);
  TEST_CHECK(ht->bucket_count == 20);
  TEST_CHECK(size_H(ht) == 20);
  for (int i = 0; i < 20; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  destroy_H(ht);
}

void test_hash_rehash_same_size(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 10; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  int old_buckets = ht->bucket_count;
  rehash_H(ht, old_buckets);
  TEST_CHECK(ht->bucket_count == old_buckets);
  TEST_CHECK(size_H(ht) == 10);
  for (int i = 0; i < 10; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  destroy_H(ht);
}

void test_hash_rehash_multiple(void) {
  HashTable *ht =
      newHashTable(2, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 1000; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == 1000);
  for (int i = 0; i < 1000; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  destroy_H(ht);
}

void test_hash_empty_operations(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  TEST_CHECK(find_H(ht, &(int){5}) == NULL);
  TEST_CHECK(erase_H(ht, &(int){5}) == 0);
  clear_H(ht);
  TEST_CHECK(size_H(ht) == 0);
  TEST_CHECK(empty_H(ht) == true);
  destroy_H(ht);
}

void test_hash_null_handling(void) {
  HashTable *ht1 = newHashTable(16, NULL, hash_func, equal_func, free_payload);
  TEST_CHECK(ht1 == NULL);
  HashTable *ht2 =
      newHashTable(16, extract_key, NULL, equal_func, free_payload);
  TEST_CHECK(ht2 == NULL);
  HashTable *ht3 = newHashTable(16, extract_key, hash_func, NULL, free_payload);
  TEST_CHECK(ht3 == NULL);
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);
  destroy_H(NULL);
  clear_H(NULL);
  empty_H(NULL);
  size_H(NULL);
  find_H(NULL, &(int){1});
  find_H(ht, NULL);
  insert_unique_H(NULL, make_payload(1, 1));
  insert_unique_H(ht, NULL);
  insert_equal_H(NULL, make_payload(1, 1));
  insert_equal_H(ht, NULL);
  erase_H(NULL, &(int){1});
  erase_H(ht, NULL);
  rehash_H(NULL, 16);
  rehash_H(ht, -1);
  destroy_H(ht);
}

void test_hash_stress(void) {
  HashTable *ht =
      newHashTable(64, extract_key, hash_func, equal_func, free_payload);
  int N = 5000;
  for (int i = 0; i < N; ++i) {
    Payload *p = make_payload(i, i);
    bool ok = insert_unique_H(ht, p);
    TEST_CHECK(ok == true);
  }
  TEST_CHECK(size_H(ht) == N);
  for (int i = 0; i < N; i += 10) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  TEST_CHECK(contains_key(ht, N + 100) == false);
  for (int i = 0; i < N / 2; ++i) {
    int erased = erase_H(ht, &i);
    TEST_CHECK(erased == 1);
  }
  TEST_CHECK(size_H(ht) == N - N / 2);
  for (int i = N / 2; i < N; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  for (int i = 0; i < N / 2; ++i) {
    Payload *p = make_payload(i + N, i + N);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == N);
  for (int i = N / 2; i < N; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  for (int i = N; i < N + N / 2; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
  }
  clear_H(ht);
  TEST_CHECK(size_H(ht) == 0);
  destroy_H(ht);
}

static void *extract_string_key(void *elem) { return elem; }
static unsigned int hash_string(void *key) {
  const char *s = (const char *)key;
  unsigned int h = 0;
  while (*s)
    h = h * 31 + *s++;
  return h;
}
static bool equal_string(void *a, void *b) {
  return strcmp((const char *)a, (const char *)b) == 0;
}
static void free_string_payload(void *elem) { free(elem); }

static bool contains_string_key(HashTable *ht, const char *key) {
  void *found = find_H(ht, (void *)key);
  if (!found)
    return false;
  return strcmp((const char *)found, key) == 0;
}

void test_hash_string_keys(void) {
  HashTable *ht = newHashTable(16, extract_string_key, hash_string,
                               equal_string, free_string_payload);
  TEST_ASSERT(ht != NULL);
  char *s1 = strdup("apple");
  char *s2 = strdup("banana");
  char *s3 = strdup("apple");
  insert_unique_H(ht, s1);
  bool ok = insert_unique_H(ht, s3);
  TEST_CHECK(ok == false);
  insert_unique_H(ht, s2);
  free(s3);
  TEST_CHECK(contains_string_key(ht, "apple") == true);
  TEST_CHECK(contains_string_key(ht, "banana") == true);
  erase_H(ht, (void *)"apple");
  TEST_CHECK(contains_string_key(ht, "apple") == false);
  TEST_CHECK(contains_string_key(ht, "banana") == true);
  destroy_H(ht);
}

void test_hash_collision(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_collision, equal_func, free_payload);
  int n = 100;
  for (int i = 0; i < n; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == n);
  List *bucket0 = GET_DA(ht->buckets, 0, List *);
  for (int i = 0; i < n; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
    TEST_CHECK(get_value(ht, i) == i);
  }
  for (int i = n; i < n + 10; ++i) {
    TEST_CHECK(contains_key(ht, i) == false);
  }
  int erased = 0;
  for (int i = 0; i < n; ++i) {
    erased += erase_H(ht, &i);
  }
  TEST_CHECK(erased == n);
  TEST_CHECK(size_H(ht) == 0);
  destroy_H(ht);
}

void test_hash_mixed_operations(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  for (int i = 0; i < 20; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  for (int i = 0; i < 20; i += 2) {
    erase_H(ht, &i);
  }
  TEST_CHECK(size_H(ht) == 10);
  for (int i = 0; i < 20; ++i) {
    if (i % 2 == 0) {
      TEST_CHECK(contains_key(ht, i) == false);
    } else {
      TEST_CHECK(contains_key(ht, i) == true);
    }
  }
  for (int i = 0; i < 10; ++i) {
    Payload *p = make_payload(i + 100, i + 100);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == 20);
  for (int i = 0; i < 10; ++i) {
    TEST_CHECK(contains_key(ht, i * 2 + 1) == true);
    TEST_CHECK(contains_key(ht, i + 100) == true);
  }
  clear_H(ht);
  TEST_CHECK(size_H(ht) == 0);
  for (int i = 0; i < 5; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
  }
  TEST_CHECK(size_H(ht) == 5);
  destroy_H(ht);
}

void test_hash_bucket_count_after_rehash(void) {
  HashTable *ht =
      newHashTable(10, extract_key, hash_func, equal_func, free_payload);
  TEST_CHECK(ht->bucket_count == 10);
  rehash_H(ht, 20);
  TEST_CHECK(ht->bucket_count == 20);
  rehash_H(ht, 5);
  TEST_CHECK(ht->bucket_count == 5);
  destroy_H(ht);
}

void test_hash_total_elements_accuracy(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_CHECK(size_H(ht) == 0);
  for (int i = 0; i < 50; ++i) {
    Payload *p = make_payload(i, i);
    insert_unique_H(ht, p);
    TEST_CHECK(size_H(ht) == i + 1);
  }
  for (int i = 0; i < 50; ++i) {
    erase_H(ht, &i);
    TEST_CHECK(size_H(ht) == 49 - i);
  }
  TEST_CHECK(size_H(ht) == 0);
  destroy_H(ht);
}

void test_hash_basic_insert_find(void) {
  HashTable *ht =
      newHashTable(16, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);

  for (int i = 0; i < 5; ++i) {
    Payload *p = make_payload(i, i * 10);
    bool ok = insert_unique_H(ht, p);
    TEST_CHECK(ok == true);
  }

  TEST_CHECK(size_H(ht) == 5);

  for (int i = 0; i < 5; ++i) {
    TEST_CHECK(contains_key(ht, i) == true);
    TEST_CHECK(get_value(ht, i) == i * 10);
  }

  TEST_CHECK(contains_key(ht, 100) == false);
  TEST_CHECK(find_H(ht, &(int){100}) == NULL);

  destroy_H(ht);
}
void test_hash_rehash_debug(void) {
  HashTable *ht =
      newHashTable(4, extract_key, hash_func, equal_func, free_payload);
  TEST_ASSERT(ht != NULL);

  for (int i = 0; i < 10; ++i) {
    Payload *p = make_payload(i, i);
    bool ok = insert_unique_H(ht, p);
    TEST_CHECK(ok == true);
  }

  for (int i = 0; i < 10; ++i) {
    int found = contains_key(ht, i);
    TEST_CHECK(found == true);
  }

  destroy_H(ht);
}

TEST_LIST = {
    {"hash_collision", test_hash_collision},
    {"hash_basic_insert_find", test_hash_basic_insert_find},
    {"hash_rehash_debug", test_hash_rehash_debug},
    {"hash_create_destroy", test_hash_create_destroy},
    {"hash_insert_unique_find", test_hash_insert_unique_find},
    {"hash_insert_unique_duplicate", test_hash_insert_unique_duplicate},
    {"hash_insert_equal", test_hash_insert_equal},
    {"hash_find_order_after_equal_insert",
     test_hash_find_order_after_equal_insert},
    {"hash_erase_single", test_hash_erase_single},
    {"hash_erase_nonexistent", test_hash_erase_nonexistent},
    {"hash_erase_middle", test_hash_erase_middle},
    {"hash_erase_head_tail", test_hash_erase_head_tail},
    {"hash_clear", test_hash_clear},
    {"hash_load_factor", test_hash_load_factor},
    {"hash_rehash_auto", test_hash_rehash_auto},
    {"hash_rehash_manual", test_hash_rehash_manual},
    {"hash_rehash_same_size", test_hash_rehash_same_size},
    {"hash_rehash_multiple", test_hash_rehash_multiple},
    {"hash_empty_operations", test_hash_empty_operations},
    {"hash_null_handling", test_hash_null_handling},
    {"hash_stress", test_hash_stress},
    {"hash_string_keys", test_hash_string_keys},
    {"hash_mixed_operations", test_hash_mixed_operations},
    {"hash_bucket_count_after_rehash", test_hash_bucket_count_after_rehash},
    {"hash_total_elements_accuracy", test_hash_total_elements_accuracy},
    {NULL, NULL}};
