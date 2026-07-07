#include "List.h"
#include "acutest.h"
#include <stdio.h>
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

void test_create_destroy(void) {
  List *list = newList();

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_L(list) == true);
  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_L(list) == NULL);
  TEST_CHECK(back_L(list) == NULL);

  clear_L(list, free_cb);

  TEST_ASSERT(list != NULL);
  TEST_CHECK(empty_L(list) == true);
  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_L(list) == NULL);
  TEST_CHECK(back_L(list) == NULL);

  destroy_L(list, free_cb);
}

void test_push_back(void) {
  List *list = newList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_back_L(list, (Node *)items[i]);
  }

  TEST_CHECK(size_L(list) == 10);
  TEST_CHECK(empty_L(list) == false);
  TEST_CHECK(front_L(list) == (Node *)items[0]);
  TEST_CHECK(back_L(list) == (Node *)items[9]);

  clear_L(list, free_cb);

  TEST_CHECK(empty_L(list) == true);
  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(front_L(list) == NULL);
  TEST_CHECK(back_L(list) == NULL);

  destroy_L(list, free_cb);
}

void test_push_front(void) {
  List *list = newList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_L(list, (Node *)items[i]);
  }

  TEST_CHECK(size_L(list) == 10);
  TEST_CHECK(empty_L(list) == false);
  TEST_CHECK(front_L(list) == (Node *)items[9]);
  TEST_CHECK(back_L(list) == (Node *)items[0]);

  destroy_L(list, free_cb);
}

void test_pop_back(void) {
  List *list = newList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_L(list, (Node *)item);
  }

  TEST_CHECK(size_L(list) == 10);
  for (int i = 10; i > 0; --i) {
    Node *node = pop_back_L(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_L(list) == i - 1);
  }

  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(empty_L(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_L(list, free_cb);
}

void test_pop_front(void) {
  List *list = newList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i + 1);
    push_back_L(list, (Node *)item);
  }

  TEST_CHECK(size_L(list) == 10);

  for (int i = 1; i <= 10; ++i) {
    Node *node = pop_front_L(list);
    TEST_CHECK(node != NULL);
    Item *it = (Item *)node;
    TEST_CHECK(it->val == i);
    free(node);
    TEST_CHECK(size_L(list) == 10 - i);
  }

  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(empty_L(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_L(list, free_cb);
}

void test_find(void) {
  List *list = newList();
  for (int i = 0; i < 100; ++i) {
    Item *item = newItem(i);
    push_back_L(list, (Node *)item);
  }
  TEST_CHECK(size_L(list) == 100);

  Node *f = find_L(list, 89);
  Node *cur = front_L(list);
  for (int i = 0; i < 89; ++i) {
    cur = cur->next;
  }

  TEST_CHECK(isequal(f, cur));
  f = find_L(list, 101);
  TEST_CHECK(f == NULL);
  f = find_L(list, -3);
  TEST_CHECK(f == NULL);
  f = find_L(list, 99);
  TEST_CHECK(isequal(back_L(list), f));
  f = find_L(list, 0);
  TEST_CHECK(isequal(front_L(list), f));
  f = find_L(list, size_L(list));
  TEST_CHECK(isequal(back_L(list), f));

  destroy_L(list, free_cb);
}

void test_insert(void) {
  List *list = newList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_L(list, (Node *)item);
  }

  TEST_CHECK(size_L(list) == 10);
  Item *it1 = newItem(101);
  insert_L(list, 5, (Node *)it1);
  TEST_CHECK(size_L(list) == 11);

  Node *cur = find_L(list, 5);

  TEST_CHECK(isequal(cur, (Node *)it1));

  Item *it2 = newItem(104);
  insert_L(list, size_L(list), (Node *)it2);
  TEST_CHECK(size_L(list) == 12);

  cur = find_L(list, size_L(list) - 1);

  TEST_CHECK(isequal(cur, (Node *)it2));

  Item *it3 = newItem(108);
  insert_L(list, 0, (Node *)it3);
  TEST_CHECK(size_L(list) == 13);

  cur = front_L(list);
  TEST_CHECK(isequal(cur, (Node *)it3));

  destroy_L(list, free_cb);
}

void test_erase(void) {
  List *list = newList();
  for (int i = 0; i < 10; ++i) {
    Item *item = newItem(i);
    push_back_L(list, (Node *)item);
  }

  TEST_CHECK(size_L(list) == 10);
  Item *it1 = (Item *)find_L(list, 5);
  TEST_CHECK(it1->val == 5);

  Item *next = (Item *)it1->node.next;
  erase_L(list, 5, free_cb);
  TEST_CHECK(next->val == 6);
  TEST_CHECK(size_L(list) == 9);

  it1 = (Item *)front_L(list);
  TEST_CHECK(it1->val == 0);
  next = (Item *)it1->node.next;
  erase_L(list, 0, free_cb);
  TEST_CHECK(next->val == 1);
  TEST_CHECK(size_L(list) == 8);

  it1 = (Item *)back_L(list);
  TEST_CHECK(it1->val == 9);
  Item *pre = (Item *)find_L(list, size_L(list) - 2);
  erase_L(list, size_L(list) - 1, free_cb);
  TEST_CHECK(pre->val == 8);
  TEST_CHECK(size_L(list) == 7);

  destroy_L(list, free_cb);
}

void test_reverse(void) {
  List *list = newList();

  int arr[10];

  for (int i = 0; i < 10; ++i) {
    arr[i] = i;
    Item *item = newItem(i);
    push_back_L(list, (Node *)item);
  }

  TEST_CHECK(size_L(list) == 10);

  reverse_L(list);

  Node *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(arr[i] == it->val);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);

  clear_L(list, free_cb);
  TEST_CHECK(size_L(list) == 0);

  reverse_L(list);
  Item *i1 = newItem(3);
  push_back_L(list, (Node *)i1);
  TEST_CHECK(size_L(list) == 1);
  reverse_L(list);

  destroy_L(list, free_cb);
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
