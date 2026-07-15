#include "stack/DStack.h"
#include "acutest.h"
#include <stdlib.h>

static int free_counter = 0;

static void test_free_cb(void *elem, void *args) {
  (void)args;
  free_counter++;
}

static void reset_counter(void) { free_counter = 0; }

void test_create_destroy(void) {
  DStack *stack = newDStack(sizeof(int), NULL, NULL);
  TEST_ASSERT(stack != NULL);
  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack) == true);

  clear_DS(stack);
  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack) == true);

  destroy_DS(stack);

  clear_DS(NULL);
  destroy_DS(NULL);
  TEST_CHECK(size_DS(NULL) == 0);
  TEST_CHECK(empty_DS(NULL) == true);

  stack = newDStack(0, NULL, NULL);
  TEST_CHECK(stack == NULL);
}

void test_push(void) {
  DStack *stack = newDStack(sizeof(int), NULL, NULL);
  TEST_ASSERT(stack != NULL);

  int v = 3;
  push_DS(stack, &v);
  TEST_CHECK(size_DS(stack) == 1);
  TEST_CHECK(empty_DS(stack) == false);

  v = 5;
  push_DS(stack, &v);
  TEST_CHECK(size_DS(stack) == 2);

  v = 7;
  push_DS(stack, &v);
  TEST_CHECK(size_DS(stack) == 3);

  for (int i = 0; i < 100; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 103);

  clear_DS(stack);
  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack) == true);

  destroy_DS(stack);
}

void test_pop(void) {
  DStack *stack = newDStack(sizeof(int), NULL, NULL);
  TEST_ASSERT(stack != NULL);

  pop_DS(stack);
  TEST_CHECK(size_DS(stack) == 0);

  for (int i = 0; i < 10; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 10);

  int v = 10;
  push_DS(stack, &v);
  TEST_CHECK(size_DS(stack) == 11);

  pop_DS(stack);
  TEST_CHECK(size_DS(stack) == 10);

  for (int i = 9; i >= 0; --i) {
    int top_val;
    top_DS(stack, &top_val);
    TEST_CHECK(top_val == i);
    pop_DS(stack);
    TEST_CHECK(size_DS(stack) == i);
  }
  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack) == true);

  pop_DS(stack);
  TEST_CHECK(size_DS(stack) == 0);

  destroy_DS(stack);
}

void test_top(void) {
  DStack *stack = newDStack(sizeof(int), NULL, NULL);
  TEST_ASSERT(stack != NULL);

  int a = 999;
  top_DS(stack, &a);
  TEST_CHECK(a == 999);

  for (int i = 0; i < 10; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 10);

  int v = 10;
  push_DS(stack, &v);
  TEST_CHECK(size_DS(stack) == 11);

  top_DS(stack, &a);
  TEST_CHECK(a == 10);

  pop_DS(stack);
  top_DS(stack, &a);
  TEST_CHECK(a == 9);

  while (!empty_DS(stack)) {
    pop_DS(stack);
  }
  a = 888;
  top_DS(stack, &a);
  TEST_CHECK(a == 888);

  destroy_DS(stack);
}

void test_free_callback(void) {
  DStack *stack = newDStack(sizeof(int), test_free_cb, NULL);
  TEST_ASSERT(stack != NULL);

  reset_counter();
  for (int i = 0; i < 5; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 5);
  TEST_CHECK(free_counter == 0);

  for (int i = 0; i < 3; ++i) {
    pop_DS(stack);
  }
  TEST_CHECK(free_counter == 3);
  TEST_CHECK(size_DS(stack) == 2);

  clear_DS(stack);
  TEST_CHECK(free_counter == 5);
  TEST_CHECK(size_DS(stack) == 0);

  destroy_DS(stack);

  stack = newDStack(sizeof(int), NULL, NULL);
  for (int i = 0; i < 3; ++i) {
    push_DS(stack, &i);
  }
  reset_counter();
  clear_DS(stack);
  TEST_CHECK(free_counter == 0);
  destroy_DS(stack);
}

void test_mixed_operations(void) {
  DStack *stack = newDStack(sizeof(int), NULL, NULL);
  TEST_ASSERT(stack != NULL);

  for (int i = 0; i < 10; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 10);

  for (int i = 0; i < 3; ++i) {
    pop_DS(stack);
  }
  TEST_CHECK(size_DS(stack) == 7);

  int a;
  top_DS(stack, &a);
  TEST_CHECK(a == 6);

  int v = 20;
  push_DS(stack, &v);
  v = 30;
  push_DS(stack, &v);
  TEST_CHECK(size_DS(stack) == 9);

  top_DS(stack, &a);
  TEST_CHECK(a == 30);
  pop_DS(stack);
  top_DS(stack, &a);
  TEST_CHECK(a == 20);
  pop_DS(stack);

  TEST_CHECK(size_DS(stack) == 7);

  clear_DS(stack);
  TEST_CHECK(size_DS(stack) == 0);

  destroy_DS(stack);
}

void test_large_volume(void) {
  DStack *stack = newDStack(sizeof(int), NULL, NULL);
  TEST_ASSERT(stack != NULL);

  const int N = 10000;
  for (int i = 0; i < N; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == N);

  for (int i = N - 1; i >= N / 2; --i) {
    int a;
    top_DS(stack, &a);
    TEST_CHECK(a == i);
    pop_DS(stack);
  }
  TEST_CHECK(size_DS(stack) == N / 2);

  for (int i = 0; i < 2000; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 5000 + 2000);

  while (!empty_DS(stack)) {
    pop_DS(stack);
  }
  TEST_CHECK(size_DS(stack) == 0);

  destroy_DS(stack);
}

void test_error_handling(void) {
  DStack *stack = newDStack(sizeof(int), NULL, NULL);
  TEST_ASSERT(stack != NULL);

  push_DS(NULL, (int[]){1});
  pop_DS(NULL);
  top_DS(NULL, NULL);
  clear_DS(NULL);
  destroy_DS(NULL);

  push_DS(stack, &(int){5});
  top_DS(stack, NULL);
  TEST_CHECK(size_DS(stack) == 1);

  push_DS(stack, NULL);
  TEST_CHECK(size_DS(stack) == 1);

  clear_DS(stack);
  destroy_DS(stack);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"push", test_push},
             {"pop", test_pop},
             {"top", test_top},
             {"free_callback", test_free_callback},
             {"mixed_operations", test_mixed_operations},
             {"large_volume", test_large_volume},
             {"error_handling", test_error_handling},
             {NULL, NULL}};
