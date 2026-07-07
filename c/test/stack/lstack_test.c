#include "LStack.h"
#include "acutest.h"
#include "common.h"
#include "config.h"

typedef struct Item {
  Node node;
  int val;
} Item;

Item *newItem(int val) {
  Item *node = malloc(sizeof(Item));
  node->node.next = NULL;
  node->val = val;
  return node;
}

void free_cb(Node *node) {
  Item *item = (Item *)node;
  free(item);
}

void test_create_destroy(void) {
  LStack *stack = newLStack();
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack));

  clear_LS(stack, free_cb);

  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack));

  destroy_LS(stack, free_cb);
}

void test_push(void) {
  LStack *stack = newLStack();
  Item *it1 = newItem(3);
  push_LS(stack, &it1->node);
  TEST_CHECK(size_LS(stack) == 1);
  TEST_CHECK(!empty_LS(stack));
  Item *it2 = newItem(5);
  push_LS(stack, &it2->node);
  TEST_CHECK(size_LS(stack) == 2);
  Item *it3 = newItem(7);
  push_LS(stack, &it3->node);
  TEST_CHECK(size_LS(stack) == 3);

  Item *it4 = newItem(9);
  push_LS(stack, &it4->node);
  TEST_CHECK(size_LS(stack) == 4);

  clear_LS(stack, free_cb);
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack));

  destroy_LS(stack, free_cb);
}

void test_pop(void) {
  LStack *stack = newLStack();

  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack));
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_LS(stack, &item->node);
  }
  TEST_CHECK(size_LS(stack) == 10);
  Item *it = newItem(11);
  push_LS(stack, &it->node);
  TEST_CHECK(size_LS(stack) == 11);

  Node *node = pop_LS(stack);
  free_cb(node);
  TEST_CHECK(size_LS(stack) == 10);

  for (int i = 10; i >= 1; --i) {
    TEST_CHECK(size_LS(stack) == i);
    Node *n = pop_LS(stack);
    free_cb(n);
  }
  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack));

  destroy_LS(stack, free_cb);
}

void test_top(void) {
  LStack *stack = newLStack();

  TEST_CHECK(size_LS(stack) == 0);
  TEST_CHECK(empty_LS(stack));
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_LS(stack, &item->node);
  }
  TEST_CHECK(size_LS(stack) == 10);
  Item *it = newItem(10);
  push_LS(stack, &it->node);
  TEST_CHECK(size_LS(stack) == 11);

  Node *n = top_LS(stack);
  TEST_CHECK(container_of(n, Item, node)->val == 10);

  for (int i = 10; i > 0; --i) {
    Node *no = pop_LS(stack);
    free_cb(no);
    Node *nod = top_LS(stack);
    TEST_CHECK(container_of(nod, Item, node)->val == i - 1);
  }

  destroy_LS(stack, free_cb);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"push", test_push},
             {"pop", test_pop},
             {"top", test_top},
             {NULL, NULL}};
