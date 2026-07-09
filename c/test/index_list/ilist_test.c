#include "IList.h"
#include "acutest.h"
#include "config.h"
#include <stdlib.h>

typedef struct Item {
  CDNode node;
  int val;
} Item;

Item *newItem(int val) {
  Item *node = malloc(sizeof(Item));
  node->node.next = NULL;
  node->val = val;
  return node;
}

void free_cb(CDNode *node) {
  Item *item = (Item *)node;
  free(item);
}

void print_cb(CDNode *node) {
  Item *it = (Item *)node;
  printf("%d", it->val);
}

bool isequal(CDNode *a, CDNode *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a;
  Item *ib = (Item *)b;

  return ia->val == ib->val;
}

int get_val(CDNode *node) {
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
    push_back_IL(list, (CDNode *)item, get_val);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(empty_IL(list) == false);
  CDNode *n = *(CDNode **)at_DA(list->array, 0);
  TEST_CHECK(isequal(n, front_IL(list)));
  n = *(CDNode **)at_DA(list->array, 9);
  TEST_CHECK(isequal(n, back_IL(list)));
  TEST_MSG("%d, %d", container_of(n, Item, node)->val,
           container_of(back_IL(list), Item, node)->val);
  n = find_CDL(list->list, 5);
  CDNode *nn = *(CDNode **)at_DA(list->array, 5);
  TEST_CHECK(isequal(n, nn));
  n = find_CDL(list->list, 7);
  nn = *(CDNode **)at_DA(list->array, 7);
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
    push_front_IL(list, (CDNode *)item, get_val);
  }
  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(empty_IL(list) == false);
  CDNode *n = front_IL(list);
  CDNode *nn = *(CDNode **)at_DA(list->array, size_IL(list) - 1);
  // n + nn = size - 1
  TEST_CHECK(isequal(n, nn));
  n = back_IL(list);
  nn = *(CDNode **)at_DA(list->array, size_IL(list) - 1 - 9);
  TEST_CHECK(isequal(n, nn));
  n = find_CDL(list->list, 5);
  nn = *(CDNode **)at_DA(list->array, size_IL(list) - 1 - 5);
  TEST_CHECK(isequal(n, nn));
  n = find_CDL(list->list, 7);
  nn = *(CDNode **)at_DA(list->array, size_IL(list) - 1 - 7);
  TEST_CHECK(isequal(n, nn));

  destroy_IL(list, free_cb);
}

void test_pop_back(void) {
  IList *list = newIList(15);

  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_IL(list, (CDNode *)item, get_val);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(size_DA(list->array) == 16);
  TEST_CHECK(empty_IL(list) == false);
  CDNode *n = back_IL(list);
  int key = get_val(n);
  CDNode *nn = *(CDNode **)at_DA(list->array, key);
  CDNode *nnn = pop_back_IL(list, get_val);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 9);
  free_cb(nnn);

  n = back_IL(list);
  key = get_val(n);
  nn = *(CDNode **)at_DA(list->array, key);
  nnn = pop_back_IL(list, get_val);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 8);
  free_cb(nnn);

  clear_IL(list, free_cb);
  TEST_CHECK(size_IL(list) == 0);

  Item *it = newItem(5);
  push_back_IL(list, (CDNode *)it, get_val);
  TEST_CHECK(size_IL(list) == 1);

  n = back_IL(list);
  key = get_val(n);
  nn = *(CDNode **)at_DA(list->array, key);
  nnn = pop_back_IL(list, get_val);

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
    push_front_IL(list, (CDNode *)item, get_val);
  }

  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(size_DA(list->array) == 16);
  TEST_CHECK(empty_IL(list) == false);
  CDNode *n = front_IL(list);
  int key = get_val(n);
  CDNode *nn = *(CDNode **)at_DA(list->array, key);
  CDNode *nnn = pop_front_IL(list, get_val);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 9);
  free_cb(nnn);

  n = front_IL(list);
  key = get_val(n);
  nn = *(CDNode **)at_DA(list->array, key);
  nnn = pop_front_IL(list, get_val);
  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 8);
  free_cb(nnn);

  clear_IL(list, free_cb);
  TEST_CHECK(size_IL(list) == 0);

  Item *it = newItem(5);
  push_front_IL(list, (CDNode *)it, get_val);
  TEST_CHECK(size_IL(list) == 1);

  n = front_IL(list);
  key = get_val(n);
  nn = *(CDNode **)at_DA(list->array, key);
  nnn = pop_back_IL(list, get_val);

  TEST_CHECK(isequal(n, nn));
  TEST_CHECK(isequal(n, nnn));
  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(size_IL(list) == 0);
  free_cb(nnn);

  destroy_IL(list, free_cb);
}

void test_insert_by_val(void) {
  IList *list = newIList(10000);

  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_IL(list, (CDNode *)item, get_val);
  }
  TEST_CHECK(size_IL(list) == 10);
  TEST_CHECK(empty_IL(list) == false);
  Item *it1 = newItem(99);
  insert_by_val_IL(list, (CDNode *)it1, 4, get_val);
  CDNode *n = find_by_val_IL(list, 4);
  CDNode *nn = find_by_val_IL(list, 99);
  TEST_CHECK(isequal(n->next, nn));
  TEST_CHECK(isequal(n, nn->previous));
  TEST_CHECK(size_IL(list) == 11);

  Item *it2 = newItem(1000);
  insert_by_val_IL(list, (CDNode *)it2, 0, get_val);
  n = find_by_val_IL(list, 0);
  nn = find_by_val_IL(list, 1000);
  TEST_CHECK(isequal(n->next, nn));
  TEST_CHECK(isequal(n, nn->previous));
  TEST_CHECK(size_IL(list) == 12);

  Item *it3 = newItem(9992);
  insert_by_val_IL(list, (CDNode *)it3, 9, get_val);
  n = find_by_val_IL(list, 9);
  nn = find_by_val_IL(list, 9992);
  CDNode *nnn = back_IL(list);

  TEST_CHECK(isequal(nn, nnn));
  TEST_CHECK(isequal(n->next, nn));
  TEST_CHECK(isequal(n, nn->previous));
  TEST_CHECK(size_IL(list) == 13);

  destroy_IL(list, free_cb);
}

// void test_erase_by_val(void);

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"push_back", test_push_back},
             {"push_front", test_push_front},
             {"pop_back", test_pop_back},
             {"pop_front", test_pop_front},
             {"insert", test_insert_by_val},
             // {"erase", test_erase_by_val},
             {NULL, NULL}};
