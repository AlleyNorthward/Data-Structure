#include "CList.h"
#include "acutest.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Item {
  CNode node;
  int val;
} Item;

Item *newItem(int val) {
  Item *node = malloc(sizeof(Item));
  node->node.next = NULL;
  node->val = val;
  return node;
}

void free_cb(CNode *node) {
  Item* item = (Item*)node;
  free(item); }

void print_cb(CNode *node) {
  Item *it = (Item *)node;
  printf("%d", it->val);
}

bool isequal(CNode *a, CNode *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a;
  Item *ib = (Item *)b;

  return ia->val == ib->val;
}

void test_create_destroy(void) {
  CList *list = newCList();

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_CL(list) == NULL);
  TEST_CHECK(back_CL(list) == NULL);

  clear_CL(list, free_cb);

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_CL(list) == NULL);
  TEST_CHECK(back_CL(list) == NULL);

  destroy_CL(list, free_cb);
}

void test_push_back(void) {
  CList *list = newCList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_back_CL(list, (CNode *)items[i]);
  }

  TEST_CHECK(size_CL(list) == 10);
  TEST_CHECK(empty_CL(list) == false);
  TEST_CHECK(front_CL(list) == (CNode *)items[0]);
  TEST_CHECK(back_CL(list) == (CNode *)items[9]);

  clear_CL(list, free_cb);

  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_CL(list) == NULL);
  TEST_CHECK(back_CL(list) == NULL);

  destroy_CL(list, free_cb);
}

void test_push_front(void) {
  CList *list = newCList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_CL(list, (CNode *)items[i]);
  }

  TEST_CHECK(size_CL(list) == 10);
  TEST_CHECK(empty_CL(list) == false);
  TEST_CHECK(front_CL(list) == (CNode *)items[9]);
  TEST_CHECK(back_CL(list) == (CNode *)items[0]);

  destroy_CL(list, free_cb);
}

void test_pop_back(void) {
  CList *list = newCList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_CL(list, (CNode *)item);
  }

  TEST_CHECK(size_CL(list) == 10);
  for (int i = 10; i > 0; --i) {
    CNode *node = pop_back_CL(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_CL(list) == i - 1);
  }

  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_CL(list, free_cb);
}

void test_pop_front(void) {
  CList *list = newCList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_CL(list, (CNode *)item);
  }

  TEST_CHECK(size_CL(list) == 10);

  for (int i = 1; i <= 10; ++i) {
    CNode *node = pop_front_CL(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_CL(list) == 10 - i);
  }

  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_CL(list, free_cb);
}

void test_find(void) {
  CList *list = newCList();
  for (int i = 0; i < 100; ++i) {
    Item *item = newItem(i);
    push_back_CL(list, (CNode *)item);
  }
  TEST_CHECK(size_CL(list) == 100);

  CNode *f = find_CL(list, 89);
  CNode *cur = front_CL(list);
  for (int i = 0; i < 89; ++i) {
    cur = cur->next;
  }

  TEST_CHECK(isequal(f, cur));
  f = find_CL(list, 101);
  TEST_CHECK(f == NULL);
  f = find_CL(list, -3);
  TEST_CHECK(f == NULL);
  f = find_CL(list, 99);
  TEST_CHECK(isequal(back_CL(list), f));
  f = find_CL(list, 0);
  TEST_CHECK(isequal(front_CL(list), f));
  f = find_CL(list, size_CL(list));
  TEST_CHECK(isequal(back_CL(list), f));

  destroy_CL(list, free_cb);
}

void test_insert(void) {
  CList *list = newCList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_CL(list, (CNode *)item);
  }

  TEST_CHECK(size_CL(list) == 10);
  Item *it1 = newItem(101);
  insert_CL(list, 5, (CNode *)it1);
  TEST_CHECK(size_CL(list) == 11);

  CNode *cur = find_CL(list, 5);

  TEST_CHECK(isequal(cur, (CNode *)it1));

  Item *it2 = newItem(104);
  insert_CL(list, size_CL(list), (CNode *)it2);
  TEST_CHECK(size_CL(list) == 12);

  cur = find_CL(list, size_CL(list) - 1);

  TEST_CHECK(isequal(cur, (CNode *)it2));

  Item *it3 = newItem(108);
  insert_CL(list, 0, (CNode *)it3);
  TEST_CHECK(size_CL(list) == 13);

  cur = front_CL(list);
  TEST_CHECK(isequal(cur, (CNode *)it3));

  destroy_CL(list, free_cb);
}

void test_erase(void) {
  CList *list = newCList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_CL(list, (CNode *)item);
  }

  TEST_CHECK(size_CL(list) == 10);
  Item *it1 = (Item *)find_CL(list, 5);
  TEST_CHECK(it1->val == 5);

  Item *next = (Item *)it1->node.next;
  erase_CL(list, 5, free_cb);
  TEST_CHECK(next->val == 6);
  TEST_CHECK(size_CL(list) == 9);

  it1 = (Item *)front_CL(list);
  TEST_CHECK(it1->val == 0);
  next = (Item *)it1->node.next;
  erase_CL(list, 0, free_cb);
  TEST_CHECK(next->val == 1);
  TEST_CHECK(size_CL(list) == 8);

  it1 = (Item *)back_CL(list);
  TEST_CHECK(it1->val == 9);
  Item *pre = (Item *)find_CL(list, size_CL(list) - 2);
  erase_CL(list, size_CL(list) - 1, free_cb);
  TEST_CHECK(pre->val == 8);
  TEST_CHECK(size_CL(list) == 7);

  destroy_CL(list, free_cb);
}

void test_reverse(void) {
  CList *list = newCList();

  int arr[10];

  for (int i = 0; i < 10; ++i) {
    arr[i] = i;
    Item *item = newItem(i);
    push_back_CL(list, (CNode *)item);
  }

  TEST_CHECK(size_CL(list) == 10);

  reverse_CL(list);

  CNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(arr[i] == it->val);
    cur = cur->next;
  }
  TEST_CHECK(isequal(cur, front_CL(list)));

  clear_CL(list, free_cb);
  TEST_CHECK(size_CL(list) == 0);

  reverse_CL(list);
  Item *i1 = newItem(3);
  push_back_CL(list, (CNode *)i1);
  TEST_CHECK(size_CL(list) == 1);
  reverse_CL(list);

  destroy_CL(list, free_cb);
}

TEST_LIST = {
    {"create_destroy", test_create_destroy},
    {"push_back", test_push_back},
    {"push_front", test_push_front},
    {"pop_back", test_pop_back},
    {"pop_front", test_pop_front},
    {"find", test_find},
    {"insert", test_insert},
    {"erase", test_erase},
    {"reverse", test_reverse},
    {NULL, NULL},
};
