#include "list/CDList.h"
#include "acutest.h"
#include <stdlib.h>

typedef struct Item {
  CDNode node;
  int val;
} Item;

Item *newItem(int val) {
  Item *it = malloc(sizeof(Item));
  it->node.next = NULL;
  it->node.previous = NULL;
  it->val = val;
  return it;
}

void free_cb(CDNode *node, void *args) {
  (void)args;
  free((Item *)node);
}

bool isequal(CDNode *a, CDNode *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a, *ib = (Item *)b;
  return ia->val == ib->val;
}

static bool check_circular_links(CDList *list) {
  if (list->head == NULL)
    return list->tail == NULL;
  if (list->tail == NULL)
    return false;
  if (list->head->previous != list->tail)
    return false;
  if (list->tail->next != list->head)
    return false;
  CDNode *cur = list->head;
  int count = 0;
  do {
    if (cur->next->previous != cur)
      return false;
    if (cur->previous->next != cur)
      return false;
    count++;
    cur = cur->next;
  } while (cur != list->head);
  return count == list->len;
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

  clear_CDL(list, free_cb, NULL);
  destroy_CDL(list, free_cb, NULL);

  list = newCDList();
  destroy_CDL(list, free_cb, NULL);
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
  TEST_CHECK(check_circular_links(list) == true);

  CDNode *cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);

  clear_CDL(list, free_cb, NULL);
  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_CDL(list, free_cb, NULL);
}

void test_push_front(void) {
  CDList *list = newCDList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_CDL(list, (CDNode *)items[i]);
  }

  TEST_CHECK(size_CDL(list) == 10);
  TEST_CHECK(front_CDL(list) == (CDNode *)items[9]);
  TEST_CHECK(back_CDL(list) == (CDNode *)items[0]);
  TEST_CHECK(check_circular_links(list) == true);

  CDNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);

  destroy_CDL(list, free_cb, NULL);
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
    if (list->tail) {
      TEST_CHECK(list->tail->next == list->head);
      TEST_CHECK(list->head->previous == list->tail);
    }
    TEST_CHECK(check_circular_links(list) == true);
  }

  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  TEST_CHECK(pop_back_CDL(list) == NULL);

  destroy_CDL(list, free_cb, NULL);
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
    if (list->head) {
      TEST_CHECK(list->head->previous == list->tail);
      TEST_CHECK(list->tail->next == list->head);
    }
    TEST_CHECK(check_circular_links(list) == true);
  }

  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(empty_CDL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  TEST_CHECK(pop_front_CDL(list) == NULL);

  destroy_CDL(list, free_cb, NULL);
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
  for (int i = 0; i < 89; ++i)
    cur = cur->next;
  TEST_CHECK(isequal(f, cur));

  TEST_CHECK(isequal(find_CDL(list, 0), front_CDL(list)));
  TEST_CHECK(isequal(find_CDL(list, 99), back_CDL(list)));

  TEST_CHECK(find_CDL(list, 100) == NULL);
  TEST_CHECK(find_CDL(list, -1) == NULL);
  TEST_CHECK(find_CDL(list, 200) == NULL);

  clear_CDL(list, free_cb, NULL);
  TEST_CHECK(find_CDL(list, 0) == NULL);

  destroy_CDL(list, free_cb, NULL);
}

void test_insert_by_pos(void) {
  CDList *list = newCDList();

  Item *it0 = newItem(100);
  insert_by_pos_CDL(list, 0, (CDNode *)it0);
  TEST_CHECK(size_CDL(list) == 1);
  TEST_CHECK(front_CDL(list) == (CDNode *)it0);
  TEST_CHECK(back_CDL(list) == (CDNode *)it0);
  TEST_CHECK(check_circular_links(list) == true);

  Item *it1 = newItem(200);
  insert_by_pos_CDL(list, 0, (CDNode *)it1);
  TEST_CHECK(size_CDL(list) == 2);
  TEST_CHECK(front_CDL(list) == (CDNode *)it1);
  TEST_CHECK(back_CDL(list) == (CDNode *)it0);
  TEST_CHECK(back_CDL(list)->next == front_CDL(list));
  TEST_CHECK(front_CDL(list)->previous == back_CDL(list));
  TEST_CHECK(check_circular_links(list) == true);

  Item *it2 = newItem(300);
  insert_by_pos_CDL(list, size_CDL(list), (CDNode *)it2);
  TEST_CHECK(size_CDL(list) == 3);
  TEST_CHECK(back_CDL(list) == (CDNode *)it2);
  TEST_CHECK(back_CDL(list)->next == front_CDL(list));
  TEST_CHECK(check_circular_links(list) == true);

  Item *it3 = newItem(250);
  insert_by_pos_CDL(list, 1, (CDNode *)it3);
  TEST_CHECK(size_CDL(list) == 4);
  CDNode *cur = list->head;
  TEST_CHECK(((Item *)cur)->val == 200);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 250);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 100);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 300);
  cur = cur->next;
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular_links(list) == true);

  int old = size_CDL(list);
  insert_by_pos_CDL(list, -1, (CDNode *)newItem(999));
  TEST_CHECK(size_CDL(list) == old);
  insert_by_pos_CDL(list, old + 5, (CDNode *)newItem(999));
  TEST_CHECK(size_CDL(list) == old);
  insert_by_pos_CDL(list, 0, NULL);
  TEST_CHECK(size_CDL(list) == old);

  destroy_CDL(list, free_cb, NULL);
}

void test_erase_by_pos(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 5; ++i) {
    Item *item = newItem(i);
    push_back_CDL(list, (CDNode *)item);
  }

  CDNode *del = erase_by_pos_CDL(list, 0);
  free(del);
  TEST_CHECK(size_CDL(list) == 4);
  TEST_CHECK(((Item *)front_CDL(list))->val == 1);
  TEST_CHECK(check_circular_links(list) == true);

  del = erase_by_pos_CDL(list, size_CDL(list) - 1);
  free(del);
  TEST_CHECK(size_CDL(list) == 3);
  TEST_CHECK(((Item *)back_CDL(list))->val == 3);
  TEST_CHECK(back_CDL(list)->next == front_CDL(list));
  TEST_CHECK(check_circular_links(list) == true);

  del = erase_by_pos_CDL(list, 1);
  free(del);
  TEST_CHECK(size_CDL(list) == 2);
  TEST_CHECK(((Item *)front_CDL(list))->val == 1);
  TEST_CHECK(((Item *)back_CDL(list))->val == 3);
  TEST_CHECK(check_circular_links(list) == true);

  del = erase_by_pos_CDL(list, 0);
  free(del);
  TEST_CHECK(size_CDL(list) == 1);
  TEST_CHECK(((Item *)front_CDL(list))->val == 3);

  del = erase_by_pos_CDL(list, 0);
  free(del);
  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  erase_by_pos_CDL(list, 0);
  TEST_CHECK(size_CDL(list) == 0);
  erase_by_pos_CDL(list, -1);
  TEST_CHECK(size_CDL(list) == 0);
  erase_by_pos_CDL(list, 5);
  TEST_CHECK(size_CDL(list) == 0);

  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_CDL(list, (CDNode *)it);
  }
  int old_len = size_CDL(list);
  erase_by_pos_CDL(list, 1);
  TEST_CHECK(size_CDL(list) == old_len - 1);
  CDNode *node = pop_front_CDL(list);
  free(node);
  node = pop_front_CDL(list);
  free(node);
  clear_CDL(list, free_cb, NULL);

  destroy_CDL(list, free_cb, NULL);
}

void test_insert_erase_prev(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_CDL(list, (CDNode *)it);
  }

  Item *newHead = newItem(-1);
  insert_CDL(list, NULL, (CDNode *)newHead);
  TEST_CHECK(size_CDL(list) == 6);
  TEST_CHECK(front_CDL(list) == (CDNode *)newHead);
  TEST_CHECK(((CDNode *)newHead)->previous == list->tail);
  TEST_CHECK(list->tail->next == (CDNode *)newHead);
  TEST_CHECK(check_circular_links(list) == true);

  Item *newTail = newItem(100);
  insert_CDL(list, list->tail, (CDNode *)newTail);
  TEST_CHECK(size_CDL(list) == 7);
  TEST_CHECK(back_CDL(list) == (CDNode *)newTail);
  TEST_CHECK(((CDNode *)newTail)->next == list->head);
  TEST_CHECK(list->head->previous == (CDNode *)newTail);
  TEST_CHECK(check_circular_links(list) == true);

  CDNode *pre = find_CDL(list, 2);
  Item *mid = newItem(50);
  insert_CDL(list, pre, (CDNode *)mid);
  TEST_CHECK(size_CDL(list) == 8);
  CDNode *cur = list->head;
  int expected[] = {-1, 0, 1, 50, 2, 3, 4, 100};
  for (int i = 0; i < 8; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular_links(list) == true);

  CDNode *del = find_CDL(list, 1);
  del = erase_CDL(list, del);
  free(del);
  TEST_CHECK(size_CDL(list) == 7);
  cur = list->head;
  int expected2[] = {-1, 1, 50, 2, 3, 4, 100};
  for (int i = 0; i < 7; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected2[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular_links(list) == true);

  del = front_CDL(list);
  del = erase_CDL(list, del);
  free(del);
  TEST_CHECK(size_CDL(list) == 6);
  TEST_CHECK(((Item *)front_CDL(list))->val == 1);

  del = back_CDL(list);
  del = erase_CDL(list, del);
  free(del);
  TEST_CHECK(size_CDL(list) == 5);
  TEST_CHECK(((Item *)back_CDL(list))->val == 4);

  del = erase_CDL(list, NULL);
  TEST_CHECK(del == NULL);
  TEST_CHECK(size_CDL(list) == 5);

  destroy_CDL(list, free_cb, NULL);
}

void test_reverse(void) {
  CDList *list = newCDList();

  reverse_CDL(list);
  TEST_CHECK(size_CDL(list) == 0);

  Item *single = newItem(1);
  push_back_CDL(list, (CDNode *)single);
  reverse_CDL(list);
  TEST_CHECK(size_CDL(list) == 1);
  TEST_CHECK(front_CDL(list) == (CDNode *)single);
  TEST_CHECK(back_CDL(list) == (CDNode *)single);
  TEST_CHECK(check_circular_links(list) == true);

  clear_CDL(list, free_cb, NULL);

  for (int i = 0; i < 10; ++i) {
    Item *it = newItem(i);
    push_back_CDL(list, (CDNode *)it);
  }
  reverse_CDL(list);
  CDNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular_links(list) == true);

  TEST_CHECK(((Item *)front_CDL(list))->val == 9);
  TEST_CHECK(((Item *)back_CDL(list))->val == 0);
  TEST_CHECK(back_CDL(list)->next == front_CDL(list));
  TEST_CHECK(front_CDL(list)->previous == back_CDL(list));

  reverse_CDL(list);
  cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular_links(list) == true);

  destroy_CDL(list, free_cb, NULL);
}

void test_clear_destroy(void) {
  CDList *list = newCDList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_CDL(list, (CDNode *)it);
  }
  TEST_CHECK(size_CDL(list) == 5);

  clear_CDL(list, free_cb, NULL);
  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  clear_CDL(list, free_cb, NULL);

  destroy_CDL(list, free_cb, NULL);

  list = newCDList();
  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_CDL(list, (CDNode *)it);
  }
  clear_CDL(list, NULL, NULL);
  TEST_CHECK(size_CDL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  free(list);

  list = newCDList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_CDL(list, (CDNode *)it);
  }
  destroy_CDL(list, free_cb, NULL);
}

void test_mixed_operations(void) {
  CDList *list = newCDList();

  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i * 2);
    push_back_CDL(list, (CDNode *)it);
  }
  Item *it100 = newItem(100);
  insert_by_pos_CDL(list, 2, (CDNode *)it100);

  CDNode *del = erase_by_pos_CDL(list, 3);
  free(del);

  Item *neg = newItem(-1);
  push_front_CDL(list, (CDNode *)neg);

  CDNode *popped = pop_back_CDL(list);
  TEST_CHECK(((Item *)popped)->val == 8);
  free(popped);

  popped = pop_front_CDL(list);
  TEST_CHECK(((Item *)popped)->val == -1);
  free(popped);

  reverse_CDL(list);

  int expected[] = {6, 100, 2, 0};
  CDNode *cur = list->head;
  for (int i = 0; i < 4; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular_links(list) == true);

  Item *mid = newItem(50);
  insert_by_pos_CDL(list, 2, (CDNode *)mid);
  TEST_CHECK(size_CDL(list) == 5);

  clear_CDL(list, free_cb, NULL);
  TEST_CHECK(size_CDL(list) == 0);

  destroy_CDL(list, free_cb, NULL);
}

void test_error_handling(void) {
  clear_CDL(NULL, free_cb, NULL);
  destroy_CDL(NULL, free_cb, NULL);
  size_CDL(NULL);
  empty_CDL(NULL);
  front_CDL(NULL);
  back_CDL(NULL);
  push_back_CDL(NULL, (CDNode *)newItem(1));
  push_front_CDL(NULL, (CDNode *)newItem(1));
  pop_back_CDL(NULL);
  pop_front_CDL(NULL);
  find_CDL(NULL, 0);
  insert_CDL(NULL, NULL, (CDNode *)newItem(1));
  erase_CDL(NULL, NULL);
  insert_by_pos_CDL(NULL, 0, (CDNode *)newItem(1));
  erase_by_pos_CDL(NULL, 0);
  reverse_CDL(NULL);

  CDList *list = newCDList();
  push_back_CDL(list, NULL);
  TEST_CHECK(size_CDL(list) == 0);
  push_front_CDL(list, NULL);
  TEST_CHECK(size_CDL(list) == 0);
  insert_CDL(list, NULL, NULL);
  TEST_CHECK(size_CDL(list) == 0);
  insert_by_pos_CDL(list, 0, NULL);
  TEST_CHECK(size_CDL(list) == 0);
  CDNode *res = erase_CDL(list, NULL);
  TEST_CHECK(res == NULL);
  TEST_CHECK(size_CDL(list) == 0);

  Item *it1 = newItem(1);
  push_back_CDL(list, (CDNode *)it1);
  CDNode *del = erase_CDL(list, (CDNode *)it1);
  free(del);
  TEST_CHECK(size_CDL(list) == 0);

  destroy_CDL(list, free_cb, NULL);
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
