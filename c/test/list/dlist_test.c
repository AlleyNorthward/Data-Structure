#include "DList.h"
#include "acutest.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Item {
  DNode node;
  int val;
} Item;

Item *newItem(int val) {
  Item *node = malloc(sizeof(Item));
  node->node.next = NULL;
  node->node.previous = NULL;
  node->val = val;
  return node;
}

void free_cb(DNode *node) {

  Item *item = (Item *)node;
  free(item);
}

void print_cb(DNode *node) {
  Item *it = (Item *)node;
  printf("%d", it->val);
}

bool isequal(DNode *a, DNode *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a;
  Item *ib = (Item *)b;

  return ia->val == ib->val;
}

void test_create_destroy(void) {
  DList *list = newDList();

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_DL(list) == NULL);
  TEST_CHECK(back_DL(list) == NULL);

  clear_DL(list, free_cb);

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_DL(list) == NULL);
  TEST_CHECK(back_DL(list) == NULL);

  destroy_DL(list, free_cb);
}

void test_push_back(void) {
  DList *list = newDList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_back_DL(list, (DNode *)items[i]);
  }

  TEST_CHECK(size_DL(list) == 10);
  TEST_CHECK(empty_DL(list) == false);
  TEST_CHECK(front_DL(list) == (DNode *)items[0]);
  TEST_CHECK(back_DL(list) == (DNode *)items[9]);

  clear_DL(list, free_cb);

  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_DL(list) == NULL);
  TEST_CHECK(back_DL(list) == NULL);

  destroy_DL(list, free_cb);
}

void test_push_front(void) {
  DList *list = newDList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_DL(list, (DNode *)items[i]);
  }

  TEST_CHECK(size_DL(list) == 10);
  TEST_CHECK(empty_DL(list) == false);
  TEST_CHECK(front_DL(list) == (DNode *)items[9]);
  TEST_CHECK(back_DL(list) == (DNode *)items[0]);

  destroy_DL(list, free_cb);
}

void test_pop_back(void) {
  DList *list = newDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_DL(list, (DNode *)item);
  }

  TEST_CHECK(size_DL(list) == 10);
  for (int i = 10; i > 0; --i) {
    DNode *node = pop_back_DL(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_DL(list) == i - 1);
  }

  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_DL(list, free_cb);
}

void test_pop_front(void) {
  DList *list = newDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_DL(list, (DNode *)item);
  }

  TEST_CHECK(size_DL(list) == 10);

  for (int i = 1; i <= 10; ++i) {
    DNode *node = pop_front_DL(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_DL(list) == 10 - i);
  }

  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_DL(list, free_cb);
}

void test_find(void) {
  DList *list = newDList();
  for (int i = 0; i < 100; ++i) {
    Item *item = newItem(i);
    push_back_DL(list, (DNode *)item);
  }
  TEST_CHECK(size_DL(list) == 100);

  DNode *f = find_DL(list, 89);
  DNode *cur = front_DL(list);
  for (int i = 0; i < 89; ++i) {
    cur = cur->next;
  }

  TEST_CHECK(isequal(f, cur));
  f = find_DL(list, 101);
  TEST_CHECK(f == NULL);
  f = find_DL(list, -3);
  TEST_CHECK(f == NULL);
  f = find_DL(list, 99);
  TEST_CHECK(isequal(back_DL(list), f));
  f = find_DL(list, 0);
  TEST_CHECK(isequal(front_DL(list), f));
  f = find_DL(list, size_DL(list) - 1);
  TEST_CHECK(isequal(back_DL(list), f));

  destroy_DL(list, free_cb);
}

void test_insert_by_pos(void) {
  DList *list = newDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_DL(list, (DNode *)item);
  }

  TEST_CHECK(size_DL(list) == 10);
  Item *it1 = newItem(101);
  insert_by_pos_DL(list, 5, (DNode *)it1);
  TEST_CHECK(size_DL(list) == 11);

  DNode *cur = find_DL(list, 5);

  TEST_CHECK(isequal(cur, (DNode *)it1));

  Item *it2 = newItem(104);
  insert_by_pos_DL(list, size_DL(list), (DNode *)it2);
  TEST_CHECK(size_DL(list) == 12);

  cur = find_DL(list, size_DL(list) - 1);

  TEST_CHECK(isequal(cur, (DNode *)it2));

  Item *it3 = newItem(108);
  insert_by_pos_DL(list, 0, (DNode *)it3);
  TEST_CHECK(size_DL(list) == 13);

  cur = front_DL(list);
  TEST_CHECK(isequal(cur, (DNode *)it3));

  destroy_DL(list, free_cb);
}

void test_erase_by_pos(void) {
  DList *list = newDList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_DL(list, (DNode *)item);
  }

  TEST_CHECK(size_DL(list) == 10);
  Item *it1 = (Item *)find_DL(list, 5);
  TEST_CHECK(it1->val == 5);

  Item *next = (Item *)it1->node.next;
  erase_by_pos_DL(list, 5, free_cb);
  TEST_CHECK(next->val == 6);
  TEST_CHECK(size_DL(list) == 9);

  it1 = (Item *)front_DL(list);
  TEST_CHECK(it1->val == 0);
  next = (Item *)it1->node.next;
  erase_by_pos_DL(list, 0, free_cb);
  TEST_CHECK(next->val == 1);
  TEST_CHECK(size_DL(list) == 8);

  it1 = (Item *)back_DL(list);
  TEST_CHECK(it1->val == 9);
  Item *pre = (Item *)find_DL(list, size_DL(list) - 2);
  erase_by_pos_DL(list, size_DL(list) - 1, free_cb);
  TEST_CHECK(pre->val == 8);
  TEST_CHECK(size_DL(list) == 7);

  destroy_DL(list, free_cb);
}

void test_reverse(void) {
  DList *list = newDList();

  int arr[10];

  for (int i = 0; i < 10; ++i) {
    arr[i] = i;
    Item *item = newItem(i);
    push_back_DL(list, (DNode *)item);
  }

  TEST_CHECK(size_DL(list) == 10);

  reverse_DL(list);

  DNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(arr[i] == it->val);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);

  clear_DL(list, free_cb);
  TEST_CHECK(size_DL(list) == 0);

  reverse_DL(list);
  Item *i1 = newItem(3);
  push_back_DL(list, (DNode *)i1);
  TEST_CHECK(size_DL(list) == 1);
  reverse_DL(list);

  destroy_DL(list, free_cb);
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
