#include "acutest.h"
#include "array/DArray.h"
#include "tree/heap.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static bool is_min_heap(void *base, int n, size_t elem_size, heap_cmp_t cmp) {
  if (n <= 1)
    return true;
  char *arr = (char *)base;
  for (int i = 0; i < (n - 1) / 2; ++i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n &&
        cmp((void *)(arr + left * elem_size), (void *)(arr + i * elem_size)))
      return false;
    if (right < n &&
        cmp((void *)(arr + right * elem_size), (void *)(arr + i * elem_size)))
      return false;
  }
  return true;
}

static bool is_max_heap(void *base, int n, size_t elem_size, heap_cmp_t cmp) {
  if (n <= 1)
    return true;
  char *arr = (char *)base;
  for (int i = 0; i < (n - 1) / 2; ++i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n &&
        cmp((void *)(arr + left * elem_size), (void *)(arr + i * elem_size)))
      return false;
    if (right < n &&
        cmp((void *)(arr + right * elem_size), (void *)(arr + i * elem_size)))
      return false;
  }
  return true;
}

static bool int_min_cmp(void *a, void *b) { return *(int *)a < *(int *)b; }
static bool int_max_cmp(void *a, void *b) { return *(int *)a > *(int *)b; }
static bool double_min_cmp(void *a, void *b) {
  double da = *(double *)a;
  double db = *(double *)b;
  return da < db;
}
static bool float_min_cmp(void *a, void *b) {
  float fa = *(float *)a;
  float fb = *(float *)b;
  return fa < fb;
}
static bool long_min_cmp(void *a, void *b) { return *(long *)a < *(long *)b; }
static bool long_max_cmp(void *a, void *b) { return *(long *)a > *(long *)b; }

typedef struct {
  int key;
  char data[32];
} KV;
static bool kv_min_cmp(void *a, void *b) {
  return ((KV *)a)->key < ((KV *)b)->key;
}

static int *random_int_array(int n, int seed) {
  int *arr = (int *)malloc(n * sizeof(int));
  if (!arr)
    return NULL;
  for (int i = 0; i < n; ++i)
    arr[i] = i;
  srand(seed);
  for (int i = n - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
  }
  return arr;
}

static float *random_float_array(int n, int seed) {
  float *arr = (float *)malloc(n * sizeof(float));
  if (!arr)
    return NULL;
  srand(seed);
  for (int i = 0; i < n; ++i)
    arr[i] = (float)rand() / RAND_MAX * 1000.0f;
  return arr;
}

void test_heap_make_min_basic(void) {
  int arr[] = {5, 3, 8, 1, 4};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
  TEST_CHECK(arr[0] == 1);
}

void test_heap_make_max_basic(void) {
  int arr[] = {5, 3, 8, 1, 4};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_max_cmp);
  TEST_CHECK(is_max_heap(arr, n, sizeof(int), int_max_cmp));
  TEST_CHECK(arr[0] == 8);
}

void test_heap_push_pop_min(void) {
  int data[10];
  int n = 0;
  int vals[] = {9, 5, 7, 3, 8, 2, 4};
  for (int i = 0; i < 7; ++i) {
    data[n++] = vals[i];
    push_heap(data, n, sizeof(int), int_min_cmp);
  }
  TEST_CHECK(is_min_heap(data, n, sizeof(int), int_min_cmp));
  pop_heap(data, n, sizeof(int), int_min_cmp);
  TEST_CHECK(data[n - 1] == 2);
  n--;
  TEST_CHECK(is_min_heap(data, n, sizeof(int), int_min_cmp));
  pop_heap(data, n, sizeof(int), int_min_cmp);
  TEST_CHECK(data[n - 1] == 3);
  n--;
  TEST_CHECK(is_min_heap(data, n, sizeof(int), int_min_cmp));
}

void test_heap_push_pop_max(void) {
  int data[10];
  int n = 0;
  int vals[] = {1, 8, 3, 9, 2, 7, 5};
  for (int i = 0; i < 7; ++i) {
    data[n++] = vals[i];
    push_heap(data, n, sizeof(int), int_max_cmp);
  }
  TEST_CHECK(is_max_heap(data, n, sizeof(int), int_max_cmp));
  pop_heap(data, n, sizeof(int), int_max_cmp);
  TEST_CHECK(data[n - 1] == 9);
  n--;
  TEST_CHECK(is_max_heap(data, n, sizeof(int), int_max_cmp));
  pop_heap(data, n, sizeof(int), int_max_cmp);
  TEST_CHECK(data[n - 1] == 8);
  n--;
  TEST_CHECK(is_max_heap(data, n, sizeof(int), int_max_cmp));
}

void test_heap_sort_min(void) {
  int arr[] = {4, 1, 3, 2, 5};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  sort_heap(arr, n, sizeof(int), int_min_cmp);
  for (int i = 0; i < n - 1; ++i)
    TEST_CHECK(arr[i] >= arr[i + 1]);
  TEST_CHECK(arr[0] == 5);
  TEST_CHECK(arr[4] == 1);
}

void test_heap_sort_max(void) {
  int arr[] = {4, 1, 3, 2, 5};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_max_cmp);
  sort_heap(arr, n, sizeof(int), int_max_cmp);
  for (int i = 0; i < n - 1; ++i)
    TEST_CHECK(arr[i] <= arr[i + 1]);
  TEST_CHECK(arr[0] == 1);
  TEST_CHECK(arr[4] == 5);
}

void test_heap_empty(void) {
  int arr[1];
  make_heap(arr, 0, sizeof(int), int_min_cmp);
  push_heap(arr, 0, sizeof(int), int_min_cmp);
  pop_heap(arr, 0, sizeof(int), int_min_cmp);
  sort_heap(arr, 0, sizeof(int), int_min_cmp);
  arr[0] = 42;
  make_heap(arr, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(arr[0] == 42);
  push_heap(arr, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(arr[0] == 42);
  pop_heap(arr, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(arr[0] == 42);
  sort_heap(arr, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(arr[0] == 42);
}

void test_heap_double_type(void) {
  double arr[] = {3.14, 1.23, 2.71, 0.58, 1.41};
  int n = 5;
  make_heap(arr, n, sizeof(double), double_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(double), double_min_cmp));
  TEST_CHECK(arr[0] == 0.58);
  double new_val = 0.1;
  double tmp[6];
  memcpy(tmp, arr, n * sizeof(double));
  tmp[n] = new_val;
  push_heap(tmp, n + 1, sizeof(double), double_min_cmp);
  TEST_CHECK(tmp[0] == 0.1);
  pop_heap(tmp, n + 1, sizeof(double), double_min_cmp);
  TEST_CHECK(tmp[n] == 0.1);
  TEST_CHECK(is_min_heap(tmp, n, sizeof(double), double_min_cmp));
}

void test_heap_float_type(void) {
  float arr[] = {3.14f, 1.23f, 2.71f, 0.58f, 1.41f};
  int n = 5;
  make_heap(arr, n, sizeof(float), float_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(float), float_min_cmp));
  TEST_CHECK(arr[0] == 0.58f);
  float new_val = 0.1f;
  float tmp[6];
  memcpy(tmp, arr, n * sizeof(float));
  tmp[n] = new_val;
  push_heap(tmp, n + 1, sizeof(float), float_min_cmp);
  TEST_CHECK(tmp[0] == 0.1f);
  pop_heap(tmp, n + 1, sizeof(float), float_min_cmp);
  TEST_CHECK(tmp[n] == 0.1f);
  TEST_CHECK(is_min_heap(tmp, n, sizeof(float), float_min_cmp));
}

void test_heap_long_type(void) {
  long arr[] = {500, 300, 800, 100, 400};
  int n = 5;
  make_heap(arr, n, sizeof(long), long_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(long), long_min_cmp));
  TEST_CHECK(arr[0] == 100);
  make_heap(arr, n, sizeof(long), long_max_cmp);
  TEST_CHECK(is_max_heap(arr, n, sizeof(long), long_max_cmp));
  TEST_CHECK(arr[0] == 800);
}

void test_heap_struct_kv(void) {
  KV arr[] = {{5, "five"}, {3, "three"}, {8, "eight"}, {1, "one"}};
  int n = 4;
  make_heap(arr, n, sizeof(KV), kv_min_cmp);
  TEST_CHECK(arr[0].key == 1);
  KV new_kv = {2, "two"};
  KV tmp[5];
  memcpy(tmp, arr, n * sizeof(KV));
  tmp[n] = new_kv;
  push_heap(tmp, n + 1, sizeof(KV), kv_min_cmp);
  TEST_CHECK(tmp[0].key == 1);
  pop_heap(tmp, n + 1, sizeof(KV), kv_min_cmp);
  TEST_CHECK(tmp[n].key == 1);
  TEST_CHECK(is_min_heap(tmp, n, sizeof(KV), kv_min_cmp));
}

void test_heap_with_darray(void) {
  DArray *da = newDArray(4, sizeof(int));
  TEST_ASSERT(da != NULL);
  int vals[] = {7, 2, 9, 4, 5, 3, 8};
  for (int i = 0; i < 7; ++i)
    push_back_DA(da, &vals[i]);
  make_heap(da->data, da->len, da->elem_size, int_min_cmp);
  TEST_CHECK(is_min_heap(da->data, da->len, sizeof(int), int_min_cmp));
  int new_val = 1;
  push_back_DA(da, &new_val);
  push_heap(da->data, da->len, da->elem_size, int_min_cmp);
  TEST_CHECK(*(int *)at_DA(da, 0) == 1);
  pop_heap(da->data, da->len, da->elem_size, int_min_cmp);
  int popped;
  memcpy(&popped, at_DA(da, da->len - 1), sizeof(int));
  TEST_CHECK(popped == 1);
  da->len--;
  TEST_CHECK(is_min_heap(da->data, da->len, sizeof(int), int_min_cmp));
  destroy_DA(da, NULL, NULL);
}

void test_heap_all_equal(void) {
  int arr[] = {5, 5, 5, 5, 5};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
  make_heap(arr, n, sizeof(int), int_max_cmp);
  TEST_CHECK(is_max_heap(arr, n, sizeof(int), int_max_cmp));
  int arr2[6] = {5, 5, 5, 5, 5, 5};
  push_heap(arr2, 6, sizeof(int), int_min_cmp);
  TEST_CHECK(is_min_heap(arr2, 6, sizeof(int), int_min_cmp));
}

void test_heap_negative_values(void) {
  int arr[] = {-5, -3, -8, -1, -4};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  TEST_CHECK(arr[0] == -8);
  TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
  make_heap(arr, n, sizeof(int), int_max_cmp);
  TEST_CHECK(arr[0] == -1);
  TEST_CHECK(is_max_heap(arr, n, sizeof(int), int_max_cmp));
}

void test_heap_reverse_sorted(void) {
  int arr[] = {5, 4, 3, 2, 1};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
  make_heap(arr, n, sizeof(int), int_max_cmp);
  TEST_CHECK(is_max_heap(arr, n, sizeof(int), int_max_cmp));
}

void test_heap_sorted(void) {
  int arr[] = {1, 2, 3, 4, 5};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
  make_heap(arr, n, sizeof(int), int_max_cmp);
  TEST_CHECK(is_max_heap(arr, n, sizeof(int), int_max_cmp));
}

void test_heap_mixed_operations(void) {
  int arr[20];
  int n = 0;
  for (int i = 0; i < 5; ++i) {
    arr[n++] = i * 2;
    push_heap(arr, n, sizeof(int), int_min_cmp);
  }
  for (int i = 0; i < 2; ++i) {
    pop_heap(arr, n, sizeof(int), int_min_cmp);
    n--;
  }
  for (int i = 0; i < 3; ++i) {
    arr[n++] = i + 10;
    push_heap(arr, n, sizeof(int), int_min_cmp);
  }
  TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
  int last = INT_MIN;
  while (n > 0) {
    pop_heap(arr, n, sizeof(int), int_min_cmp);
    int val = arr[n - 1];
    TEST_CHECK(val >= last);
    last = val;
    n--;
  }
}

void test_heap_null_safety(void) {
  make_heap(NULL, 5, sizeof(int), int_min_cmp);
  push_heap(NULL, 5, sizeof(int), int_min_cmp);
  pop_heap(NULL, 5, sizeof(int), int_min_cmp);
  sort_heap(NULL, 5, sizeof(int), int_min_cmp);
  int arr[5];
  make_heap(arr, 5, sizeof(int), NULL);
  push_heap(arr, 5, sizeof(int), NULL);
  pop_heap(arr, 5, sizeof(int), NULL);
  sort_heap(arr, 5, sizeof(int), NULL);
  make_heap(arr, -1, sizeof(int), int_min_cmp);
  make_heap(arr, 0, sizeof(int), int_min_cmp);
  TEST_CHECK(1);
}

void test_heap_edge_one_element(void) {
  int a = 99;
  make_heap(&a, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(a == 99);
  pop_heap(&a, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(a == 99);
  push_heap(&a, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(a == 99);
  sort_heap(&a, 1, sizeof(int), int_min_cmp);
  TEST_CHECK(a == 99);
}

void test_heap_edge_duplicate_sequence(void) {
  int arr[] = {2, 2, 2, 2, 2, 2, 2};
  int n = 7;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
  make_heap(arr, n, sizeof(int), int_max_cmp);
  TEST_CHECK(is_max_heap(arr, n, sizeof(int), int_max_cmp));
  sort_heap(arr, n, sizeof(int), int_min_cmp);
  for (int i = 0; i < n - 1; ++i)
    TEST_CHECK(arr[i] >= arr[i + 1]);
}

void test_heap_stress_large_build(void) {
  int N = 100000;
  int *arr = (int *)malloc(N * sizeof(int));
  TEST_ASSERT(arr != NULL);
  srand(777);
  for (int i = 0; i < N; ++i)
    arr[i] = rand() % (N * 10);
  make_heap(arr, N, sizeof(int), int_min_cmp);
  TEST_CHECK(is_min_heap(arr, N, sizeof(int), int_min_cmp));
  sort_heap(arr, N, sizeof(int), int_min_cmp);
  for (int i = 0; i < N - 1; ++i)
    TEST_CHECK(arr[i] >= arr[i + 1]);
  free(arr);
}

void test_heap_stress_random_ops(void) {
  int N = 20000;
  int *arr = (int *)malloc(N * sizeof(int));
  TEST_ASSERT(arr != NULL);
  int n = 0;
  srand(12345);
  for (int i = 0; i < N; ++i) {
    arr[n++] = rand();
    push_heap(arr, n, sizeof(int), int_min_cmp);
    if (n > 0 && (rand() % 3 == 0)) {
      pop_heap(arr, n, sizeof(int), int_min_cmp);
      n--;
      if (n > 0)
        TEST_CHECK(is_min_heap(arr, n, sizeof(int), int_min_cmp));
    }
  }
  while (n > 0) {
    pop_heap(arr, n, sizeof(int), int_min_cmp);
    n--;
  }
  free(arr);
}

void test_heap_stress_float_random(void) {
  int N = 5000;
  float *arr = random_float_array(N, 999);
  TEST_ASSERT(arr != NULL);
  make_heap(arr, N, sizeof(float), float_min_cmp);
  TEST_CHECK(is_min_heap(arr, N, sizeof(float), float_min_cmp));
  sort_heap(arr, N, sizeof(float), float_min_cmp);
  for (int i = 0; i < N - 1; ++i)
    TEST_CHECK(arr[i] >= arr[i + 1]);
  free(arr);
}

void test_heap_rebuild_after_sort(void) {
  int arr[] = {9, 7, 5, 3, 1};
  int n = 5;
  make_heap(arr, n, sizeof(int), int_min_cmp);
  sort_heap(arr, n, sizeof(int), int_min_cmp);
  make_heap(arr, n, sizeof(int), int_max_cmp);
  TEST_CHECK(is_max_heap(arr, n, sizeof(int), int_max_cmp));
  TEST_CHECK(arr[0] == 9);
}

void test_heap_dynamic_growth(void) {
  DArray *da = newDArray(2, sizeof(int));
  TEST_ASSERT(da != NULL);
  int val;
  for (int i = 0; i < 1000; ++i) {
    val = rand() % 10000;
    push_back_DA(da, &val);
    push_heap(da->data, da->len, da->elem_size, int_min_cmp);
    if (i % 3 == 0) {
      pop_heap(da->data, da->len, da->elem_size, int_min_cmp);
      da->len--;
    }
  }
  TEST_CHECK(is_min_heap(da->data, da->len, sizeof(int), int_min_cmp));
  destroy_DA(da, NULL, NULL);
}

TEST_LIST = {
    {"heap_make_min_basic", test_heap_make_min_basic},
    {"heap_make_max_basic", test_heap_make_max_basic},
    {"heap_push_pop_min", test_heap_push_pop_min},
    {"heap_push_pop_max", test_heap_push_pop_max},
    {"heap_sort_min", test_heap_sort_min},
    {"heap_sort_max", test_heap_sort_max},
    {"heap_empty", test_heap_empty},
    {"heap_double_type", test_heap_double_type},
    {"heap_float_type", test_heap_float_type},
    {"heap_long_type", test_heap_long_type},
    {"heap_struct_kv", test_heap_struct_kv},
    {"heap_with_darray", test_heap_with_darray},
    {"heap_all_equal", test_heap_all_equal},
    {"heap_negative_values", test_heap_negative_values},
    {"heap_reverse_sorted", test_heap_reverse_sorted},
    {"heap_sorted", test_heap_sorted},
    {"heap_mixed_operations", test_heap_mixed_operations},
    {"heap_null_safety", test_heap_null_safety},
    {"heap_edge_one_element", test_heap_edge_one_element},
    {"heap_edge_duplicate_sequence", test_heap_edge_duplicate_sequence},
    {"heap_stress_large_build", test_heap_stress_large_build},
    {"heap_stress_random_ops", test_heap_stress_random_ops},
    {"heap_stress_float_random", test_heap_stress_float_random},
    {"heap_rebuild_after_sort", test_heap_rebuild_after_sort},
    {"heap_dynamic_growth", test_heap_dynamic_growth},
    {NULL, NULL}};
