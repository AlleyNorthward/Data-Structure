#include "acutest.h"
#include "queue/PQueue.h"
#include <limits.h>
#include <stdlib.h>

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

static bool int_min_cmp(void *a, void *b) { return *(int *)a < *(int *)b; }
static bool int_max_cmp(void *a, void *b) { return *(int *)a > *(int *)b; }
static bool double_min_cmp(void *a, void *b) {
  return *(double *)a < *(double *)b;
}
static bool double_max_cmp(void *a, void *b) {
  return *(double *)a > *(double *)b;
}

typedef struct {
  int key;
  char data[32];
} KV;
static bool kv_min_cmp(void *a, void *b) {
  return ((KV *)a)->key < ((KV *)b)->key;
}
static bool kv_max_cmp(void *a, void *b) {
  return ((KV *)a)->key > ((KV *)b)->key;
}

static bool int_ptr_min_cmp(void *a, void *b) {
  int *pa = *(int **)a;
  int *pb = *(int **)b;
  return *pa < *pb;
}

static void int_free_cb(void *elem, void *args) {
  (void)args;
  free(*(int **)elem);
}

void test_pq_new_destroy(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  TEST_CHECK(pq != NULL);
  TEST_CHECK(empty_PQ(pq));
  TEST_CHECK(size_PQ(pq) == 0);
  TEST_CHECK(top_PQ(pq) == NULL);
  destroy_PQ(pq);
}

void test_pq_push_top_min(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[] = {5, 3, 8, 1, 4};
  int current_min = INT_MAX;
  for (int i = 0; i < 5; ++i) {
    if (vals[i] < current_min)
      current_min = vals[i];
    push_PQ(pq, &vals[i]);
    TEST_CHECK(*(int *)top_PQ(pq) == current_min);
  }
  TEST_CHECK(size_PQ(pq) == 5);
  destroy_PQ(pq);
}

void test_pq_push_top_max(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_max_cmp, NULL, NULL);
  int vals[] = {5, 3, 8, 1, 4};
  int current_max = INT_MIN;
  for (int i = 0; i < 5; ++i) {
    if (vals[i] > current_max)
      current_max = vals[i];
    push_PQ(pq, &vals[i]);
    TEST_CHECK(*(int *)top_PQ(pq) == current_max);
  }
  TEST_CHECK(size_PQ(pq) == 5);
  destroy_PQ(pq);
}

void test_pq_pop_order_min(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[] = {5, 3, 8, 1, 4};
  for (int i = 0; i < 5; ++i)
    push_PQ(pq, &vals[i]);

  int expected[] = {1, 3, 4, 5, 8};
  for (int i = 0; i < 5; ++i) {
    TEST_CHECK(*(int *)top_PQ(pq) == expected[i]);
    pop_PQ(pq);
  }
  TEST_CHECK(empty_PQ(pq));
  TEST_CHECK(size_PQ(pq) == 0);
  destroy_PQ(pq);
}

void test_pq_pop_order_max(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_max_cmp, NULL, NULL);
  int vals[] = {5, 3, 8, 1, 4};
  for (int i = 0; i < 5; ++i)
    push_PQ(pq, &vals[i]);

  int expected[] = {8, 5, 4, 3, 1};
  for (int i = 0; i < 5; ++i) {
    TEST_CHECK(*(int *)top_PQ(pq) == expected[i]);
    pop_PQ(pq);
  }
  TEST_CHECK(empty_PQ(pq));
  destroy_PQ(pq);
}

void test_pq_empty_operations(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  TEST_CHECK(empty_PQ(pq));
  TEST_CHECK(top_PQ(pq) == NULL);
  pop_PQ(pq);
  TEST_CHECK(empty_PQ(pq));
  destroy_PQ(pq);
}

void test_pq_single_element(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int val = 42;
  push_PQ(pq, &val);
  TEST_CHECK(*(int *)top_PQ(pq) == 42);
  TEST_CHECK(size_PQ(pq) == 1);
  pop_PQ(pq);
  TEST_CHECK(empty_PQ(pq));
  destroy_PQ(pq);
}

void test_pq_duplicate_elements(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[] = {5, 5, 3, 3, 3};
  for (int i = 0; i < 5; ++i)
    push_PQ(pq, &vals[i]);

  int expected[] = {3, 3, 3, 5, 5};
  for (int i = 0; i < 5; ++i) {
    TEST_CHECK(*(int *)top_PQ(pq) == expected[i]);
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_negative_values(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[] = {-5, -3, -8, -1, -4};
  for (int i = 0; i < 5; ++i)
    push_PQ(pq, &vals[i]);

  int expected[] = {-8, -5, -4, -3, -1};
  for (int i = 0; i < 5; ++i) {
    TEST_CHECK(*(int *)top_PQ(pq) == expected[i]);
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_double_type(void) {
  PQueue *pq = newPQueue(4, sizeof(double), double_min_cmp, NULL, NULL);
  double vals[] = {3.14, 1.23, 2.71, 0.58, 1.41};
  for (int i = 0; i < 5; ++i)
    push_PQ(pq, &vals[i]);

  double expected[] = {0.58, 1.23, 1.41, 2.71, 3.14};
  for (int i = 0; i < 5; ++i) {
    TEST_CHECK(*(double *)top_PQ(pq) == expected[i]);
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_struct_kv_min(void) {
  PQueue *pq = newPQueue(4, sizeof(KV), kv_min_cmp, NULL, NULL);
  KV vals[] = {{5, "five"}, {3, "three"}, {8, "eight"}, {1, "one"}};
  for (int i = 0; i < 4; ++i)
    push_PQ(pq, &vals[i]);

  int expected_keys[] = {1, 3, 5, 8};
  for (int i = 0; i < 4; ++i) {
    KV *top = (KV *)top_PQ(pq);
    TEST_CHECK(top->key == expected_keys[i]);
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_struct_kv_max(void) {
  PQueue *pq = newPQueue(4, sizeof(KV), kv_max_cmp, NULL, NULL);
  KV vals[] = {{5, "five"}, {3, "three"}, {8, "eight"}, {1, "one"}};
  for (int i = 0; i < 4; ++i)
    push_PQ(pq, &vals[i]);

  int expected_keys[] = {8, 5, 3, 1};
  for (int i = 0; i < 4; ++i) {
    KV *top = (KV *)top_PQ(pq);
    TEST_CHECK(top->key == expected_keys[i]);
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_with_free_cb(void) {
  PQueue *pq = newPQueue(4, sizeof(int *), int_ptr_min_cmp, int_free_cb, NULL);
  int *p1 = malloc(sizeof(int));
  *p1 = 5;
  int *p2 = malloc(sizeof(int));
  *p2 = 3;
  int *p3 = malloc(sizeof(int));
  *p3 = 8;

  push_PQ(pq, &p1);
  push_PQ(pq, &p2);
  push_PQ(pq, &p3);

  TEST_CHECK(**(int **)top_PQ(pq) == 3);
  destroy_PQ(pq);
}

void test_pq_clear(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[] = {5, 3, 8, 1, 4};
  for (int i = 0; i < 5; ++i)
    push_PQ(pq, &vals[i]);
  TEST_CHECK(size_PQ(pq) == 5);

  clear_PQ(pq);
  TEST_CHECK(empty_PQ(pq));
  TEST_CHECK(size_PQ(pq) == 0);
  TEST_CHECK(top_PQ(pq) == NULL);

  int new_val = 10;
  push_PQ(pq, &new_val);
  TEST_CHECK(*(int *)top_PQ(pq) == 10);
  destroy_PQ(pq);
}

void test_pq_auto_expand(void) {
  PQueue *pq = newPQueue(2, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[100];
  for (int i = 0; i < 100; ++i)
    vals[i] = i;
  for (int i = 0; i < 100; ++i)
    push_PQ(pq, &vals[i]);

  TEST_CHECK(size_PQ(pq) == 100);
  TEST_CHECK(*(int *)top_PQ(pq) == 0);

  for (int i = 0; i < 100; ++i) {
    TEST_CHECK(*(int *)top_PQ(pq) == i);
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_mixed_operations(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  for (int i = 0; i < 10; ++i) {
    int val = (i * 7) % 10;
    push_PQ(pq, &val);
    if (i % 3 == 0 && !empty_PQ(pq)) {
      pop_PQ(pq);
    }
  }
  TEST_CHECK(*(int *)top_PQ(pq) >= 0);
  TEST_CHECK(size_PQ(pq) <= 10);
  destroy_PQ(pq);
}

void test_pq_interleaved_push_pop(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[] = {9, 5, 7, 3, 8, 2, 4};
  int popped[7];
  int pop_idx = 0;

  for (int i = 0; i < 7; ++i) {
    push_PQ(pq, &vals[i]);
    if (i % 2 == 0 && !empty_PQ(pq)) {
      popped[pop_idx++] = *(int *)top_PQ(pq);
      pop_PQ(pq);
    }
  }

  while (!empty_PQ(pq)) {
    popped[pop_idx++] = *(int *)top_PQ(pq);
    pop_PQ(pq);
  }

  TEST_CHECK(pop_idx == 7);
  destroy_PQ(pq);
}

void test_pq_stress_large_push(void) {
  int N = 100000;
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);

  for (int i = 0; i < N; ++i) {
    int val = rand() % (N * 10);
    push_PQ(pq, &val);
  }
  TEST_CHECK(size_PQ(pq) == N);

  int last = INT_MIN;
  while (!empty_PQ(pq)) {
    int cur = *(int *)top_PQ(pq);
    TEST_CHECK(cur >= last);
    last = cur;
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_stress_random_ops(void) {
  int N = 50000;
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  srand(12345);

  for (int i = 0; i < N; ++i) {
    int val = rand() % 10000;
    push_PQ(pq, &val);
    if (rand() % 3 == 0 && !empty_PQ(pq)) {
      pop_PQ(pq);
    }
  }

  int last = INT_MIN;
  while (!empty_PQ(pq)) {
    int cur = *(int *)top_PQ(pq);
    TEST_CHECK(cur >= last);
    last = cur;
    pop_PQ(pq);
  }
  destroy_PQ(pq);
}

void test_pq_stress_clear_reuse(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  for (int round = 0; round < 10; ++round) {
    for (int i = 0; i < 1000; ++i) {
      int val = rand() % 10000;
      push_PQ(pq, &val);
    }
    TEST_CHECK(size_PQ(pq) == 1000);
    clear_PQ(pq);
    TEST_CHECK(empty_PQ(pq));
  }
  destroy_PQ(pq);
}

void test_pq_stress_alternating(void) {
  int N = 20000;
  srand(12345);

  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int *push_vals = malloc(N * sizeof(int));
  int *pop_vals = malloc(N * sizeof(int));
  int push_cnt = 0, pop_cnt = 0;

  for (int i = 0; i < N; ++i) {
    if (rand() % 2 == 0) {
      push_vals[push_cnt++] = rand() % 1000;
      push_PQ(pq, &push_vals[push_cnt - 1]);
    } else if (!empty_PQ(pq)) {
      pop_PQ(pq);
      if (!empty_PQ(pq)) {
        TEST_CHECK(
            is_min_heap(pq->arr->data, pq->arr->len, sizeof(int), int_min_cmp));
      }
    }
  }

  while (!empty_PQ(pq)) {
    pop_vals[pop_cnt++] = *(int *)top_PQ(pq);
    pop_PQ(pq);
    if (!empty_PQ(pq)) {
      TEST_CHECK(
          is_min_heap(pq->arr->data, pq->arr->len, sizeof(int), int_min_cmp));
    }
  }

  for (int i = 1; i < pop_cnt; ++i) {
    TEST_CHECK(pop_vals[i] >= pop_vals[i - 1]);
  }

  free(push_vals);
  free(pop_vals);
  destroy_PQ(pq);
}

void test_pq_with_darray_raw_ops(void) {
  PQueue *pq = newPQueue(4, sizeof(int), int_min_cmp, NULL, NULL);
  int vals[] = {7, 2, 9, 4, 5, 3, 8};
  for (int i = 0; i < 7; ++i) {
    push_back_DA(pq->arr, &vals[i]);
  }
  make_heap(pq->arr->data, pq->arr->len, pq->arr->elem_size, int_min_cmp);
  TEST_CHECK(*(int *)top_PQ(pq) == 2);

  int new_val = 1;
  push_PQ(pq, &new_val);
  TEST_CHECK(*(int *)top_PQ(pq) == 1);

  pop_PQ(pq);
  TEST_CHECK(*(int *)top_PQ(pq) == 2);
  destroy_PQ(pq);
}

TEST_LIST = {{"pq_new_destroy", test_pq_new_destroy},
             {"pq_push_top_min", test_pq_push_top_min},
             {"pq_push_top_max", test_pq_push_top_max},
             {"pq_pop_order_min", test_pq_pop_order_min},
             {"pq_pop_order_max", test_pq_pop_order_max},
             {"pq_empty_operations", test_pq_empty_operations},
             {"pq_single_element", test_pq_single_element},
             {"pq_duplicate_elements", test_pq_duplicate_elements},
             {"pq_negative_values", test_pq_negative_values},
             {"pq_double_type", test_pq_double_type},
             {"pq_struct_kv_min", test_pq_struct_kv_min},
             {"pq_struct_kv_max", test_pq_struct_kv_max},
             {"pq_with_free_cb", test_pq_with_free_cb},
             {"pq_clear", test_pq_clear},
             {"pq_auto_expand", test_pq_auto_expand},
             {"pq_mixed_operations", test_pq_mixed_operations},
             {"pq_interleaved_push_pop", test_pq_interleaved_push_pop},
             {"pq_stress_large_push", test_pq_stress_large_push},
             {"pq_stress_random_ops", test_pq_stress_random_ops},
             {"pq_stress_clear_reuse", test_pq_stress_clear_reuse},
             {"pq_stress_alternating", test_pq_stress_alternating},
             {"pq_with_darray_raw_ops", test_pq_with_darray_raw_ops},
             {NULL, NULL}};
