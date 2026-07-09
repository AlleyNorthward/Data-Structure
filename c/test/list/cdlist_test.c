#include "CDList.h"
#include "acutest.h"
#include <stdio.h>
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

void test_create_destroy(void) {
  CDList *list = newCDList();

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_CDL(list) == NULL);
  TEST_CHECK(back_CDL(list) == NULL);

  clear_CDL(list, free_cb);

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_CDL(list) == NULL);
  TEST_CHECK(back_CDL(list) == NULL);

  destroy_CDL(list, free_cb);
}

void test_push_back(void) {
  CDList *list = newCDList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_back_CDL(list, (CDNode *)items[i]);
  }

  TEST_CHECK(size_CDL(list) == 10);
  TEST_CHECK(empty_CDL(list) == false);
  TEST_CHECK(front_CDL(list) == (CDNode *)items[0]);
  TEST_CHECK(back_CDL(list) == (CDNode *)items[9]);

  clear_CDL(list, free_cb);

  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_CDL(list) == NULL);
  TEST_CHECK(back_CDL(list) == NULL);

  destroy_CDL(list, free_cb);
}

void test_push_front(void) {
  CDList *list = newCDList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_CDL(list, (CDNode *)items[i]);
  }

  TEST_CHECK(size_CDL(list) == 10);
  TEST_CHECK(empty_CDL(list) == false);
  TEST_CHECK(front_CDL(list) == (CDNode *)items[9]);
  TEST_CHECK(back_CDL(list) == (CDNode *)items[0]);

  destroy_CDL(list, free_cb);
}

void test_pop_back(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_CDL(list, (CDNode *)item);
  }

  TEST_CHECK(size_CDL(list) == 10);
  for (int i = 10; i > 0; --i) {
    CDNode *node = pop_back_CDL(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_CDL(list) == i - 1);
  }

  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_CDL(list, free_cb);
}

void test_pop_front(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_CDL(list, (CDNode *)item);
  }

  TEST_CHECK(size_CDL(list) == 10);

  for (int i = 1; i <= 10; ++i) {
    CDNode *node = pop_front_CDL(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_CDL(list) == 10 - i);
  }

  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_CDL(list, free_cb);
}

void test_find(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 100; ++i) {
    Item *item = newItem(i);
    push_back_CDL(list, (CDNode *)item);
  }
  TEST_CHECK(size_CDL(list) == 100);

  CDNode *f = find_CDL(list, 89);
  CDNode *cur = front_CDL(list);
  for (int i = 0; i < 89; ++i) {
    cur = cur->next;
  }

  TEST_CHECK(isequal(f, cur));
  f = find_CDL(list, 101);
  TEST_CHECK(f == NULL);
  f = find_CDL(list, -3);
  TEST_CHECK(f == NULL);
  f = find_CDL(list, 99);
  TEST_CHECK(isequal(back_CDL(list), f));
  f = find_CDL(list, 0);
  TEST_CHECK(isequal(front_CDL(list), f));
  f = find_CDL(list, size_CDL(list) - 1);
  TEST_CHECK(isequal(back_CDL(list), f));

  destroy_CDL(list, free_cb);
}

void test_insert_by_pos(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_CDL(list, (CDNode *)item);
  }

  TEST_CHECK(size_CDL(list) == 10);
  Item *it1 = newItem(101);
  insert_by_pos_CDL(list, 5, (CDNode *)it1);
  TEST_CHECK(size_CDL(list) == 11);

  CDNode *cur = find_CDL(list, 5);

  TEST_CHECK(isequal(cur, (CDNode *)it1));

  Item *it2 = newItem(104);
  insert_by_pos_CDL(list, size_CDL(list), (CDNode *)it2);
  TEST_CHECK(size_CDL(list) == 12);

  cur = find_CDL(list, size_CDL(list) - 1);

  TEST_CHECK(isequal(cur, (CDNode *)it2));

  Item *it3 = newItem(108);
  insert_by_pos_CDL(list, 0, (CDNode *)it3);
  TEST_CHECK(size_CDL(list) == 13);

  cur = front_CDL(list);
  TEST_CHECK(isequal(cur, (CDNode *)it3));

  destroy_CDL(list, free_cb);
}

void test_erase_by_pos(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_CDL(list, (CDNode *)item);
  }

  TEST_CHECK(size_CDL(list) == 10);
  Item *it1 = (Item *)find_CDL(list, 5);
  TEST_CHECK(it1->val == 5);

  Item *next = (Item *)it1->node.next;
  erase_by_pos_CDL(list, 5, free_cb);
  TEST_CHECK(next->val == 6);
  TEST_CHECK(size_CDL(list) == 9);

  it1 = (Item *)front_CDL(list);
  TEST_CHECK(it1->val == 0);
  next = (Item *)it1->node.next;
  erase_by_pos_CDL(list, 0, free_cb);
  TEST_CHECK(next->val == 1);
  TEST_CHECK(size_CDL(list) == 8);

  it1 = (Item *)back_CDL(list);
  TEST_CHECK(it1->val == 9);
  Item *pre = (Item *)find_CDL(list, size_CDL(list) - 2);
  erase_by_pos_CDL(list, size_CDL(list) - 1, free_cb);
  TEST_CHECK(pre->val == 8);
  TEST_CHECK(size_CDL(list) == 7);

  destroy_CDL(list, free_cb);
}

void test_reverse(void) {
  CDList *list = newCDList();

  int arr[10];

  for (int i = 0; i < 10; ++i) {
    arr[i] = i;
    Item *item = newItem(i);
    push_back_CDL(list, (CDNode *)item);
  }

  TEST_CHECK(size_CDL(list) == 10);

  reverse_CDL(list);

  CDNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(arr[i] == it->val);
    cur = cur->next;
  }
  TEST_CHECK(isequal(cur, front_CDL(list)));

  clear_CDL(list, free_cb);
  TEST_CHECK(size_CDL(list) == 0);

  reverse_CDL(list);
  Item *i1 = newItem(3);
  push_back_CDL(list, (CDNode *)i1);
  TEST_CHECK(size_CDL(list) == 1);
  reverse_CDL(list);

  destroy_CDL(list, free_cb);
}

TEST_LIST = {
    {"create_destroy", test_create_destroy},
    {"push_back", test_push_back},
    {"push_front", test_push_front},
    {"pop_back", test_pop_back},
    {"pop_front", test_pop_front},
    {"find", test_find},
    {"insert", test_insert_by_pos},
    {"erase", test_erase_by_pos},
    {"reverse", test_reverse},
    {NULL, NULL},
};
