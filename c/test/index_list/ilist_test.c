#include "IList.h"
#include "acutest.h"
#include "config.h"
#include <stdlib.h>

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

void print_cb(Node *node) {
  Item *it = (Item *)node;
  printf("%d", it->val);
}

bool isequal(Node *a, Node *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a;
  Item *ib = (Item *)b;

  return ia->val == ib->val;
}

int get_key(Node *node) {
  Item *it = container_of(node, Item, node);
  return it->val;
}

void test_create_destroy(void) {
  IList *list = newIList(10);

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_IL(list) == true);
  TEST_CHECK(size_IL(list) == 0);
  TEST_CHECK(front_IL(list) == NULL);
  TEST_CHECK(back_IL(list) == NULL);

  clear_IL(list, free_cb);

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_IL(list) == true);
  TEST_CHECK(size_IL(list) == 0);
  TEST_CHECK(front_IL(list) == NULL);
  TEST_CHECK(back_IL(list) == NULL);

  destroy_IL(list, free_cb);
}

void test_push_back(void) {
  IList *list = newIList(15);

  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_IL(list, (Node *)item, get_key);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(empty_IL(list) == false);
  Node *n = *(Node **)at_DA(list->array, 0);
  TEST_CHECK(isequal(n, front_IL(list)));
  n = *(Node **)at_DA(list->array, 9);
  TEST_CHECK(isequal(n, back_IL(list)));
  TEST_MSG("%d, %d", container_of(n, Item, node)->val,
           container_of(back_IL(list), Item, node)->val);
  n = find_L(list->list, 5);
  Node *nn = *(Node **)at_DA(list->array, 5);
  TEST_CHECK(isequal(n, nn));
  n = find_L(list->list, 7);
  nn = *(Node **)at_DA(list->array, 7);
  TEST_CHECK(isequal(n, nn));

  TEST_CHECK(size_DA(list->array) == 16);

  clear_IL(list, free_cb);
  TEST_CHECK(empty_IL(list));
  TEST_CHECK(size_IL(list) == 0);
  TEST_CHECK(front_IL(list) == NULL);
  TEST_CHECK(back_IL(list) == NULL);

  destroy_IL(list, free_cb);
}

void test_push_front(void) {
  IList *list = newIList(15);
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_front_IL(list, (Node *)item, get_key);
  }
  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(empty_IL(list) == false);
  Node *n = front_IL(list);
  Node *nn = *(Node **)at_DA(list->array, size_IL(list) - 1);
  // n + nn = size - 1
  TEST_CHECK(isequal(n, nn));
  n = back_IL(list);
  nn = *(Node **)at_DA(list->array, size_IL(list) - 1 - 9);
  TEST_CHECK(isequal(n, nn));
  n = find_L(list->list, 5);
  nn = *(Node **)at_DA(list->array, size_IL(list) - 1 - 5);
  TEST_CHECK(isequal(n, nn));
  n = find_L(list->list, 7);
  nn = *(Node **)at_DA(list->array, size_IL(list) - 1 - 7);
  TEST_CHECK(isequal(n, nn));

  destroy_IL(list, free_cb);
}

void test_pop_back(void) {
  IList *list = newIList(15);

  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_IL(list, (Node *)item, get_key);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(size_DA(list->array) == 16);
  TEST_CHECK(empty_IL(list) == false);
  Node *n = back_IL(list);
  int key = get_key(n);
  Node *nn = *(Node **)at_DA(list->array, key);
  Node *nnn = pop_back_IL(list, get_key);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 9);
  free_cb(nnn);

  n = back_IL(list);
  key = get_key(n);
  nn = *(Node **)at_DA(list->array, key);
  nnn = pop_back_IL(list, get_key);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 8);
  free_cb(nnn);

  clear_IL(list, free_cb);
  TEST_CHECK(size_IL(list) == 0);

  Item *it = newItem(5);
  push_back_IL(list, (Node *)it, get_key);
  TEST_CHECK(size_IL(list) == 1);

  n = back_IL(list);
  key = get_key(n);
  nn = *(Node **)at_DA(list->array, key);
  nnn = pop_back_IL(list, get_key);

  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 0);
  free_cb(nnn);

  destroy_IL(list, free_cb);
}

void test_pop_front(void) {
  IList *list = newIList(15);
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_front_IL(list, (Node *)item, get_key);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(size_DA(list->array) == 16);
  TEST_CHECK(empty_IL(list) == false);
  Node *n = front_IL(list);
  int key = get_key(n);
  Node *nn = *(Node **)at_DA(list->array, key);
  Node *nnn = pop_front_IL(list, get_key);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 9);
  free_cb(nnn);

  n = front_IL(list);
  key = get_key(n);
  nn = *(Node **)at_DA(list->array, key);
  nnn = pop_front_IL(list, get_key);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 8);
  free_cb(nnn);

  clear_IL(list, free_cb);
  TEST_CHECK(size_IL(list) == 0);

  Item *it = newItem(5);
  push_front_IL(list, (Node *)it, get_key);
  TEST_CHECK(size_IL(list) == 1);

  n = front_IL(list);
  key = get_key(n);
  nn = *(Node **)at_DA(list->array, key);
  nnn = pop_back_IL(list, get_key);

  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 0);
  free_cb(nnn);

  destroy_IL(list, free_cb);
}

void test_insert(void) {
  IList *list = newIList(15);

  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_IL(list, (Node *)item, get_key);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(size_DA(list->array) == 16);
  TEST_CHECK(empty_IL(list) == false);
  Node *pre = *(Node **)at_DA(list->array, 4);
  Node *next = *(Node **)at_DA(list->array, 5);

  Item *item = newItem(12);
  insert_IL(list, (Node *)item, 5, get_key);
  Node *n = *(Node **)at_DA(list->array, 5);
  TEST_CHECK(container_of(pre->next, Item, node)->val == 12);
  TEST_CHECK(isequal(n, next));
  TEST_CHECK(size_IL(list) == 11);

  next = front_IL(list);
  Item *item2 = newItem(13);
  insert_IL(list, (Node *)item2, 0, get_key);
  n = *(Node **)at_DA(list->array, 0);
  TEST_CHECK(isequal(n, next));
  TEST_CHECK(size_IL(list) == 12);

  pre = back_IL(list);
  Item *item3 = newItem(14);
  insert_IL(list, (Node *)item3, size_IL(list), get_key);
  n = back_IL(list);
  TEST_CHECK(container_of(pre->next, Item, node)->val == 14);
  TEST_CHECK(size_IL(list) == 13);

  destroy_IL(list, free_cb);
}

void test_erase(void) {
  IList *list = newIList(15);

  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_IL(list, (Node *)item, get_key);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(size_DA(list->array) == 16);
  TEST_CHECK(empty_IL(list) == false);

  Node *del = GET_DA(list->array, 5, Node *);
  Node *pre = GET_DA(list->array, 4, Node *);
  Node *next = del->next;
  TEST_CHECK(isequal(del->next, next));
  erase_IL(list, 5, free_cb, get_key);
  Node *cur = GET_DA(list->array, 5, Node *);
  TEST_CHECK(cur == NULL);
  TEST_CHECK(isequal(pre->next, next));
  TEST_CHECK(size_IL(list) == 9);

  del = GET_DA(list->array, 0, Node *);
  next = del->next;
  erase_IL(list, 0, free_cb, get_key);
  cur = GET_DA(list->array, 0, Node *);
  TEST_CHECK(cur == NULL);
  TEST_CHECK(isequal(front_IL(list), next));
  TEST_CHECK(size_IL(list) == 8);

  del = GET_DA(list->array, size_IL(list) - 1, Node *);
  pre = GET_DA(list->array, 8, Node *);
  erase_IL(list, size_IL(list) - 1, free_cb, get_key);
  TEST_CHECK(isequal(back_IL(list), pre));
  TEST_CHECK(size_IL(list) == 7);

  destroy_IL(list, free_cb);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"push_back", test_push_back},
             {"push_front", test_push_front},
             {"pop_back", test_pop_back},
             {"pop_front", test_pop_front},
             {"insert", test_insert},
             {"erase", test_erase},
             {NULL, NULL}};
