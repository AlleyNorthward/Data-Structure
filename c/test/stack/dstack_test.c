#include "DStack.h"
#include "acutest.h"

void test_create_destroy(void) {
  DStack *stack = newDStack(sizeof(int), NULL);
  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack));

  clear_DS(stack);

  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack));

  destroy_DS(stack);
}

void test_push(void) {
  DStack *stack = newDStack(sizeof(int), NULL);
  PUSH_INT_DS(stack, 3);
  TEST_CHECK(size_DS(stack) == 1);
  TEST_CHECK(!empty_DS(stack));
  PUSH_INT_DS(stack, 2);
  TEST_CHECK(size_DS(stack) == 2);
  PUSH_INT_DS(stack, 5);
  TEST_CHECK(size_DS(stack) == 3);

  push_DS(stack, &(int){22});
  TEST_CHECK(size_DS(stack) == 4);

  clear_DS(stack);
  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack));

  destroy_DS(stack);
}

void test_pop(void) {
  DStack *stack = newDStack(sizeof(int), NULL);

  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack));
  for (int i = 0; i < 10; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 10);
  PUSH_INT_DS(stack, 10);
  TEST_CHECK(size_DS(stack) == 11);

  pop_DS(stack);
  TEST_CHECK(size_DS(stack) == 10);

  for (int i = 10; i >= 1; --i) {
    TEST_CHECK(size_DS(stack) == i);
    pop_DS(stack);
  }
  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack));

  destroy_DS(stack);
}

void test_top(void) {
  DStack *stack = newDStack(sizeof(int), NULL);

  TEST_CHECK(size_DS(stack) == 0);
  TEST_CHECK(empty_DS(stack));
  for (int i = 0; i < 10; ++i) {
    push_DS(stack, &i);
  }
  TEST_CHECK(size_DS(stack) == 10);
  PUSH_INT_DS(stack, 10);
  TEST_CHECK(size_DS(stack) == 11);

  int a;
  top_DS(stack, &a);
  TEST_CHECK(a == 10);

  for (int i = 10; i > 0; --i) {
    pop_DS(stack);
    top_DS(stack, &a);
    TEST_CHECK(a == i - 1);
  }

  destroy_DS(stack);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"push", test_push},
             {"pop", test_pop},
             {"top", test_top},
             {NULL, NULL}};
