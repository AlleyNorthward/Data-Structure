#include "list/List.h"
#include "acutest.h"
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
  free((Item *)node);
}

bool isequal(Node *a, Node *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a, *ib = (Item *)b;
  return ia->val == ib->val;
}

static bool list_contains(List *list, int val) {
  Node *cur = list->head;
  while (cur) {
    if (((Item *)cur)->val == val)
      return true;
    cur = cur->next;
  }
  return false;
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

  clear_L(list, free_cb, NULL);
  destroy_L(list, free_cb, NULL);

  list = newList();
  destroy_L(list, free_cb, NULL);
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

  Node *cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);

  clear_L(list, free_cb, NULL);
  TEST_CHECK(empty_L(list) == true);
  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_L(list, free_cb, NULL);
}

void test_push_front(void) {
  List *list = newList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_L(list, (Node *)items[i]);
  }

  TEST_CHECK(size_L(list) == 10);
  TEST_CHECK(front_L(list) == (Node *)items[9]);
  TEST_CHECK(back_L(list) == (Node *)items[0]);

  Node *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);

  destroy_L(list, free_cb, NULL);
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

  TEST_CHECK(pop_back_L(list) == NULL);

  destroy_L(list, free_cb, NULL);
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

  TEST_CHECK(pop_front_L(list) == NULL);

  destroy_L(list, free_cb, NULL);
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
  for (int i = 0; i < 89; ++i)
    cur = cur->next;
  TEST_CHECK(isequal(f, cur));

  TEST_CHECK(isequal(find_L(list, 0), front_L(list)));
  TEST_CHECK(isequal(find_L(list, 99), back_L(list)));

  TEST_CHECK(find_L(list, 100) == NULL);
  TEST_CHECK(find_L(list, -1) == NULL);
  TEST_CHECK(find_L(list, 200) == NULL);

  clear_L(list, free_cb, NULL);
  TEST_CHECK(find_L(list, 0) == NULL);

  destroy_L(list, free_cb, NULL);
}

void test_insert_by_pos(void) {
  List *list = newList();

  Item *it0 = newItem(100);
  insert_by_pos_L(list, 0, (Node *)it0);
  TEST_CHECK(size_L(list) == 1);
  TEST_CHECK(front_L(list) == (Node *)it0);
  TEST_CHECK(back_L(list) == (Node *)it0);

  Item *it1 = newItem(200);
  insert_by_pos_L(list, 0, (Node *)it1);
  TEST_CHECK(size_L(list) == 2);
  TEST_CHECK(front_L(list) == (Node *)it1);
  TEST_CHECK(front_L(list)->next == (Node *)it0);

  Item *it2 = newItem(300);
  insert_by_pos_L(list, size_L(list), (Node *)it2);
  TEST_CHECK(size_L(list) == 3);
  TEST_CHECK(back_L(list) == (Node *)it2);

  Item *it3 = newItem(250);
  insert_by_pos_L(list, 1, (Node *)it3);
  TEST_CHECK(size_L(list) == 4);
  Node *cur = list->head;
  TEST_CHECK(((Item *)cur)->val == 200);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 250);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 100);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 300);
  cur = cur->next;
  TEST_CHECK(cur == NULL);

  int old = size_L(list);
  insert_by_pos_L(list, -1, (Node *)newItem(999));
  TEST_CHECK(size_L(list) == old);
  insert_by_pos_L(list, old + 5, (Node *)newItem(999));
  TEST_CHECK(size_L(list) == old);

  insert_by_pos_L(list, 0, NULL);
  TEST_CHECK(size_L(list) == old);

  destroy_L(list, free_cb, NULL);
}

void test_erase_by_pos(void) {
  List *list = newList();
  for (int i = 0; i < 5; ++i) {
    Item *item = newItem(i);
    push_back_L(list, (Node *)item);
  }

  Node *del = erase_by_pos_L(list, 0);
  free(del);
  TEST_CHECK(size_L(list) == 4);
  TEST_CHECK(((Item *)front_L(list))->val == 1);

  del = erase_by_pos_L(list, size_L(list) - 1);
  free(del);
  TEST_CHECK(size_L(list) == 3);
  TEST_CHECK(((Item *)back_L(list))->val == 3);

  del = erase_by_pos_L(list, 1);
  free(del);
  TEST_CHECK(size_L(list) == 2);
  TEST_CHECK(((Item *)front_L(list))->val == 1);
  TEST_CHECK(((Item *)back_L(list))->val == 3);

  del = erase_by_pos_L(list, 0);
  free(del);
  TEST_CHECK(size_L(list) == 1);
  TEST_CHECK(((Item *)front_L(list))->val == 3);

  del = erase_by_pos_L(list, 0);
  free(del);
  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  erase_by_pos_L(list, 0);
  TEST_CHECK(size_L(list) == 0);
  erase_by_pos_L(list, -1);
  TEST_CHECK(size_L(list) == 0);
  erase_by_pos_L(list, 5);
  TEST_CHECK(size_L(list) == 0);

  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_L(list, (Node *)it);
  }
  int old_len = size_L(list);
  erase_by_pos_L(list, 1);
  TEST_CHECK(size_L(list) == old_len - 1);
  Node *node = pop_front_L(list);
  free(node);
  node = pop_front_L(list);
  free(node);
  clear_L(list, free_cb, NULL);

  destroy_L(list, free_cb, NULL);
}

void test_insert_erase_prev(void) {
  List *list = newList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_L(list, (Node *)it);
  }

  Item *newHead = newItem(-1);
  insert_L(list, NULL, (Node *)newHead);
  TEST_CHECK(size_L(list) == 6);
  TEST_CHECK(front_L(list) == (Node *)newHead);

  Item *newTail = newItem(100);
  insert_L(list, list->tail, (Node *)newTail);
  TEST_CHECK(size_L(list) == 7);
  TEST_CHECK(back_L(list) == (Node *)newTail);

  Node *pre = find_L(list, 2);
  Item *mid = newItem(50);
  insert_L(list, pre, (Node *)mid);
  TEST_CHECK(size_L(list) == 8);
  Node *cur = list->head;
  int expected[] = {-1, 0, 1, 50, 2, 3, 4, 100};
  for (int i = 0; i < 8; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }

  pre = find_L(list, 1);
  Node *del = erase_L(list, pre);
  free(del);
  TEST_CHECK(size_L(list) == 7);
  cur = list->head;
  int expected2[] = {-1, 0, 50, 2, 3, 4, 100};
  for (int i = 0; i < 7; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected2[i]);
    cur = cur->next;
  }

  del = erase_L(list, NULL);
  free(del);
  TEST_CHECK(size_L(list) == 6);
  TEST_CHECK(((Item *)front_L(list))->val == 0);

  pre = find_L(list, size_L(list) - 2);
  del = erase_L(list, pre);
  free(del);
  TEST_CHECK(size_L(list) == 5);
  TEST_CHECK(((Item *)back_L(list))->val == 4);

  pre = back_L(list);
  del = erase_L(list, pre);
  TEST_CHECK(del == NULL);
  TEST_CHECK(size_L(list) == 5);

  destroy_L(list, free_cb, NULL);
}

void test_reverse(void) {
  List *list = newList();

  reverse_L(list);
  TEST_CHECK(size_L(list) == 0);

  Item *single = newItem(1);
  push_back_L(list, (Node *)single);
  reverse_L(list);
  TEST_CHECK(size_L(list) == 1);
  TEST_CHECK(front_L(list) == (Node *)single);
  TEST_CHECK(back_L(list) == (Node *)single);

  clear_L(list, free_cb, NULL);

  for (int i = 0; i < 10; ++i) {
    Item *it = newItem(i);
    push_back_L(list, (Node *)it);
  }
  reverse_L(list);
  Node *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);

  TEST_CHECK(((Item *)front_L(list))->val == 9);
  TEST_CHECK(((Item *)back_L(list))->val == 0);

  reverse_L(list);
  cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }

  destroy_L(list, free_cb, NULL);
}

void test_clear_destroy(void) {
  List *list = newList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_L(list, (Node *)it);
  }
  TEST_CHECK(size_L(list) == 5);

  clear_L(list, free_cb, NULL);
  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  clear_L(list, free_cb, NULL);

  destroy_L(list, free_cb, NULL);

  list = newList();
  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_L(list, (Node *)it);
  }
  clear_L(list, NULL, NULL);
  TEST_CHECK(size_L(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  free(list);

  list = newList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_L(list, (Node *)it);
  }
  destroy_L(list, free_cb, NULL);
}

void test_mixed_operations(void) {
  List *list = newList();

  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i * 2);
    push_back_L(list, (Node *)it);
  }
  Item *it100 = newItem(100);
  insert_by_pos_L(list, 2, (Node *)it100);

  Node *del = erase_by_pos_L(list, 3);
  free(del);

  Item *neg = newItem(-1);
  push_front_L(list, (Node *)neg);

  Node *popped = pop_back_L(list);
  TEST_CHECK(((Item *)popped)->val == 8);
  free(popped);

  popped = pop_front_L(list);
  TEST_CHECK(((Item *)popped)->val == -1);
  free(popped);

  reverse_L(list);

  int expected[] = {6, 100, 2, 0};
  Node *cur = list->head;
  for (int i = 0; i < 4; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);

  Item *mid = newItem(50);
  insert_by_pos_L(list, 2, (Node *)mid);
  TEST_CHECK(size_L(list) == 5);

  clear_L(list, free_cb, NULL);
  TEST_CHECK(size_L(list) == 0);

  destroy_L(list, free_cb, NULL);
}

void test_error_handling(void) {
  clear_L(NULL, free_cb, NULL);
  destroy_L(NULL, free_cb, NULL);
  size_L(NULL);
  empty_L(NULL);
  front_L(NULL);
  back_L(NULL);
  push_back_L(NULL, (Node *)newItem(1));
  push_front_L(NULL, (Node *)newItem(1));
  pop_back_L(NULL);
  pop_front_L(NULL);
  find_L(NULL, 0);
  insert_L(NULL, NULL, (Node *)newItem(1));
  erase_L(NULL, NULL);
  insert_by_pos_L(NULL, 0, (Node *)newItem(1));
  erase_by_pos_L(NULL, 0);
  reverse_L(NULL);

  List *list = newList();
  push_back_L(list, NULL);
  TEST_CHECK(size_L(list) == 0);
  push_front_L(list, NULL);
  TEST_CHECK(size_L(list) == 0);
  insert_L(list, NULL, NULL);
  TEST_CHECK(size_L(list) == 0);
  insert_by_pos_L(list, 0, NULL);
  TEST_CHECK(size_L(list) == 0);
  destroy_L(list, free_cb, NULL);
}

TEST_LIST = {{"create_destroy", test_create_destroy},
             {"push_back", test_push_back},
             {"push_front", test_push_front},
             {"pop_back", test_pop_back},
             {"pop_front", test_pop_front},
             {"find", test_find},
             {"insert_by_pos", test_insert_by_pos},
             {"erase_by_pos", test_erase_by_pos},
             {"insert_erase_prev", test_insert_erase_prev},
             {"reverse", test_reverse},
             {"clear_destroy", test_clear_destroy},
             {"mixed_operations", test_mixed_operations},
             {"error_handling", test_error_handling},
             {NULL, NULL}};
