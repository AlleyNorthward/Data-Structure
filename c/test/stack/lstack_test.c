#include "stack/LStack.h"
#include "acutest.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct Item {
  Node node;
  int val;
} Item;

Item *newItem(int val) {
  Item *it = malloc(sizeof(Item));
  it->node.next = NULL;
  it->val = val;
  return it;
}

void free_cb(Node *node, void *args) {
  (void)args;
  if (node)
    free((Item *)node);
}

void test_create_destroy(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack) == true);

  clear_LS(stack, free_cb, NULL);
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack) == true);

  destroy_LS(stack, free_cb, NULL);

  stack = newLStack();
  TEST_ASSERT(stack != NULL);
  destroy_LS(stack, free_cb, NULL);

  clear_LS(NULL, free_cb, NULL);
  destroy_LS(NULL, free_cb, NULL);
  TEST_CHECK(size_LS(NULL) == 0);
  TEST_CHECK(empty_LS(NULL) == true);
}

void test_push(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);

  Item *it1 = newItem(3);
  push_LS(stack, &it1->node);
  TEST_CHECK(size_LS(stack) == 1);
  TEST_CHECK(empty_LS(stack) == false);

  Item *it2 = newItem(5);
  push_LS(stack, &it2->node);
  TEST_CHECK(size_LS(stack) == 2);

  Item *it3 = newItem(7);
  push_LS(stack, &it3->node);
  TEST_CHECK(size_LS(stack) == 3);

  for (int i = 0; i < 100; ++i) {
    Item *it = newItem(i);
    push_LS(stack, &it->node);
  }
  TEST_CHECK(size_LS(stack) == 103);

  clear_LS(stack, free_cb, NULL);
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack) == true);

  destroy_LS(stack, free_cb, NULL);
}

void test_pop(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);

  Node *n = pop_LS(stack);
  TEST_CHECK(n == NULL);

  for (int i = 0; i < 10; ++i) {
    Item *it = newItem(i);
    push_LS(stack, &it->node);
  }
  TEST_CHECK(size_LS(stack) == 10);

  Item *it10 = newItem(10);
  push_LS(stack, &it10->node);
  TEST_CHECK(size_LS(stack) == 11);

  Node *pop_node = pop_LS(stack);
  TEST_CHECK(pop_node != NULL);
  TEST_CHECK(((Item *)pop_node)->val == 10);
  free_cb(pop_node, NULL);
  TEST_CHECK(size_LS(stack) == 10);

  for (int i = 9; i >= 0; --i) {
    pop_node = pop_LS(stack);
    TEST_CHECK(pop_node != NULL);
    TEST_CHECK(((Item *)pop_node)->val == i);
    free_cb(pop_node, NULL);
    TEST_CHECK(size_LS(stack) == i);
  }
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack) == true);

  pop_node = pop_LS(stack);
  TEST_CHECK(pop_node == NULL);

  destroy_LS(stack, free_cb, NULL);
}

void test_top(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);

  TEST_CHECK(top_LS(stack) == NULL);

  for (int i = 0; i < 10; ++i) {
    Item *it = newItem(i);
    push_LS(stack, &it->node);
  }
  TEST_CHECK(size_LS(stack) == 10);

  Node *top_node = top_LS(stack);
  TEST_CHECK(top_node != NULL);
  TEST_CHECK(((Item *)top_node)->val == 9);

  Item *it100 = newItem(100);
  push_LS(stack, &it100->node);
  top_node = top_LS(stack);
  TEST_CHECK(top_node != NULL);
  TEST_CHECK(((Item *)top_node)->val == 100);

  Node *pop_node = pop_LS(stack);
  free_cb(pop_node, NULL);
  top_node = top_LS(stack);
  TEST_CHECK(top_node != NULL);
  TEST_CHECK(((Item *)top_node)->val == 9);

  while (!empty_LS(stack)) {
    pop_node = pop_LS(stack);
    free_cb(pop_node, NULL);
  }
  TEST_CHECK(top_LS(stack) == NULL);

  destroy_LS(stack, free_cb, NULL);
}

void test_clear_destroy_with_callback(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);

  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_LS(stack, &it->node);
  }
  TEST_CHECK(size_LS(stack) == 5);

  clear_LS(stack, free_cb, NULL);
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack) == true);

  clear_LS(stack, free_cb, NULL);

  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_LS(stack, &it->node);
  }
  destroy_LS(stack, free_cb, NULL);

  LStack *stack2 = newLStack();
  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_LS(stack2, &it->node);
  }
  clear_LS(stack2, NULL, NULL);
  TEST_CHECK(size_LS(stack2) == 0);
  TEST_CHECK(empty_LS(stack2) == true);
  while (!empty_LS(stack2)) {
    Node *n = pop_LS(stack2);
    free_cb(n, NULL);
  }
  free(stack2);
}

void test_mixed_operations(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);

  for (int i = 0; i < 10; ++i) {
    Item *it = newItem(i);
    push_LS(stack, &it->node);
  }

  for (int i = 0; i < 3; ++i) {
    Node *n = pop_LS(stack);
    free_cb(n, NULL);
  }
  TEST_CHECK(size_LS(stack) == 7);

  Node *top = top_LS(stack);
  TEST_CHECK(top != NULL && ((Item *)top)->val == 6);

  Item *it20 = newItem(20);
  push_LS(stack, &it20->node);
  Item *it30 = newItem(30);
  push_LS(stack, &it30->node);
  TEST_CHECK(size_LS(stack) == 9);

  Node *n = pop_LS(stack);
  TEST_CHECK(((Item *)n)->val == 30);
  free_cb(n, NULL);
  n = pop_LS(stack);
  TEST_CHECK(((Item *)n)->val == 20);
  free_cb(n, NULL);

  clear_LS(stack, free_cb, NULL);
  TEST_CHECK(size_LS(stack) == 0);

  destroy_LS(stack, free_cb, NULL);
}

void test_large_volume(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);

  const int N = 10000;
  for (int i = 0; i < N; ++i) {
    Item *it = newItem(i);
    push_LS(stack, &it->node);
  }
  TEST_CHECK(size_LS(stack) == N);

  for (int i = 0; i < N / 2; ++i) {
    Node *n = pop_LS(stack);
    TEST_CHECK(((Item *)n)->val == N - 1 - i);
    free_cb(n, NULL);
  }
  TEST_CHECK(size_LS(stack) == N / 2);

  for (int i = 0; i < 2000; ++i) {
    Item *it = newItem(i + 1000);
    push_LS(stack, &it->node);
  }
  TEST_CHECK(size_LS(stack) == 5000 + 2000);

  while (!empty_LS(stack)) {
    Node *n = pop_LS(stack);
    free_cb(n, NULL);
  }
  TEST_CHECK(size_LS(stack) == 0);

  destroy_LS(stack, free_cb, NULL);
}

void test_error_handling(void) {
  LStack *stack = newLStack();
  TEST_ASSERT(stack != NULL);

  push_LS(NULL, (Node *)newItem(1));
  pop_LS(NULL);
  top_LS(NULL);
  clear_LS(NULL, free_cb, NULL);
  destroy_LS(NULL, free_cb, NULL);
  size_LS(NULL);
  empty_LS(NULL);

  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack) == true);

  push_LS(stack, NULL);
  TEST_CHECK(size_LS(stack) == 0);

  destroy_LS(stack, free_cb, NULL);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"push", test_push},
             {"pop", test_pop},
             {"top", test_top},
             {"clear_destroy_with_callback", test_clear_destroy_with_callback},
             {"mixed_operations", test_mixed_operations},
             {"large_volume", test_large_volume},
             {"error_handling", test_error_handling},
             {NULL, NULL}};
