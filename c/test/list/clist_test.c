#include "list/CList.h"
#include "acutest.h"
#include <stdlib.h>

typedef struct Item {
  CNode node;
  int val;
} Item;

Item *newItem(int val) {
  Item *it = malloc(sizeof(Item));
  it->node.next = NULL;
  it->val = val;
  return it;
}

void free_cb(CNode *node, void *args) {
  (void)args;
  free((Item *)node);
}

bool isequal(CNode *a, CNode *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a, *ib = (Item *)b;
  return ia->val == ib->val;
}

static bool list_contains(CList *list, int val) {
  if (list->head == NULL)
    return false;
  CNode *cur = list->head;
  do {
    if (((Item *)cur)->val == val)
      return true;
    cur = cur->next;
  } while (cur != list->head);
  return false;
}

static bool check_circular(CList *list) {
  if (list->head == NULL)
    return list->tail == NULL;
  if (list->tail == NULL)
    return false;
  if (list->tail->next != list->head)
    return false;
  if (list->head == list->tail) {
    return list->head->next == list->head;
  }
  CNode *cur = list->head;
  int count = 0;
  do {
    count++;
    cur = cur->next;
  } while (cur != list->head);
  return count == list->len;
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

  clear_CL(list, free_cb, NULL);
  destroy_CL(list, free_cb, NULL);

  list = newCList();
  destroy_CL(list, free_cb, NULL);
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
  TEST_CHECK(check_circular(list) == true);

  CNode *cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);

  clear_CL(list, free_cb, NULL);
  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  TEST_CHECK(check_circular(list) == true);

  destroy_CL(list, free_cb, NULL);
}

void test_push_front(void) {
  CList *list = newCList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_CL(list, (CNode *)items[i]);
  }

  TEST_CHECK(size_CL(list) == 10);
  TEST_CHECK(front_CL(list) == (CNode *)items[9]);
  TEST_CHECK(back_CL(list) == (CNode *)items[0]);
  TEST_CHECK(check_circular(list) == true);

  CNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);

  destroy_CL(list, free_cb, NULL);
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
    if (list->tail) {
      TEST_CHECK(list->tail->next == list->head);
    }
    TEST_CHECK(check_circular(list) == true);
  }

  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  TEST_CHECK(pop_back_CL(list) == NULL);

  destroy_CL(list, free_cb, NULL);
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
    if (list->head) {
      TEST_CHECK(list->tail->next == list->head);
    }
    TEST_CHECK(check_circular(list) == true);
  }

  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(empty_CL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  TEST_CHECK(pop_front_CL(list) == NULL);

  destroy_CL(list, free_cb, NULL);
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
  for (int i = 0; i < 89; ++i)
    cur = cur->next;
  TEST_CHECK(isequal(f, cur));

  TEST_CHECK(isequal(find_CL(list, 0), front_CL(list)));
  TEST_CHECK(isequal(find_CL(list, 99), back_CL(list)));

  TEST_CHECK(find_CL(list, 100) == NULL);
  TEST_CHECK(find_CL(list, -1) == NULL);
  TEST_CHECK(find_CL(list, 200) == NULL);

  clear_CL(list, free_cb, NULL);
  TEST_CHECK(find_CL(list, 0) == NULL);

  destroy_CL(list, free_cb, NULL);
}

void test_insert_by_pos(void) {
  CList *list = newCList();

  Item *it0 = newItem(100);
  insert_by_pos_CL(list, 0, (CNode *)it0);
  TEST_CHECK(size_CL(list) == 1);
  TEST_CHECK(front_CL(list) == (CNode *)it0);
  TEST_CHECK(back_CL(list) == (CNode *)it0);
  TEST_CHECK(check_circular(list) == true);

  Item *it1 = newItem(200);
  insert_by_pos_CL(list, 0, (CNode *)it1);
  TEST_CHECK(size_CL(list) == 2);
  TEST_CHECK(front_CL(list) == (CNode *)it1);
  TEST_CHECK(back_CL(list) == (CNode *)it0);
  TEST_CHECK(back_CL(list)->next == front_CL(list));
  TEST_CHECK(check_circular(list) == true);

  Item *it2 = newItem(300);
  insert_by_pos_CL(list, size_CL(list), (CNode *)it2);
  TEST_CHECK(size_CL(list) == 3);
  TEST_CHECK(back_CL(list) == (CNode *)it2);
  TEST_CHECK(back_CL(list)->next == front_CL(list));
  TEST_CHECK(check_circular(list) == true);

  Item *it3 = newItem(250);
  insert_by_pos_CL(list, 1, (CNode *)it3);
  TEST_CHECK(size_CL(list) == 4);
  CNode *cur = list->head;
  TEST_CHECK(((Item *)cur)->val == 200);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 250);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 100);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 300);
  cur = cur->next;
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular(list) == true);

  int old = size_CL(list);
  insert_by_pos_CL(list, -1, (CNode *)newItem(999));
  TEST_CHECK(size_CL(list) == old);
  insert_by_pos_CL(list, old + 5, (CNode *)newItem(999));
  TEST_CHECK(size_CL(list) == old);
  insert_by_pos_CL(list, 0, NULL);
  TEST_CHECK(size_CL(list) == old);

  destroy_CL(list, free_cb, NULL);
}

void test_erase_by_pos(void) {
  CList *list = newCList();
  for (int i = 0; i < 5; ++i) {
    Item *item = newItem(i);
    push_back_CL(list, (CNode *)item);
  }

  CNode *del = erase_by_pos_CL(list, 0);
  free(del);
  TEST_CHECK(size_CL(list) == 4);
  TEST_CHECK(((Item *)front_CL(list))->val == 1);
  TEST_CHECK(check_circular(list) == true);

  del = erase_by_pos_CL(list, size_CL(list) - 1);
  free(del);
  TEST_CHECK(size_CL(list) == 3);
  TEST_CHECK(((Item *)back_CL(list))->val == 3);
  TEST_CHECK(back_CL(list)->next == front_CL(list));
  TEST_CHECK(check_circular(list) == true);

  del = erase_by_pos_CL(list, 1);
  free(del);
  TEST_CHECK(size_CL(list) == 2);
  TEST_CHECK(((Item *)front_CL(list))->val == 1);
  TEST_CHECK(((Item *)back_CL(list))->val == 3);
  TEST_CHECK(check_circular(list) == true);

  del = erase_by_pos_CL(list, 0);
  free(del);
  TEST_CHECK(size_CL(list) == 1);
  TEST_CHECK(((Item *)front_CL(list))->val == 3);

  del = erase_by_pos_CL(list, 0);
  free(del);
  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  erase_by_pos_CL(list, 0);
  TEST_CHECK(size_CL(list) == 0);
  erase_by_pos_CL(list, -1);
  TEST_CHECK(size_CL(list) == 0);
  erase_by_pos_CL(list, 5);
  TEST_CHECK(size_CL(list) == 0);

  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }
  int old_len = size_CL(list);
  erase_by_pos_CL(list, 1);
  TEST_CHECK(size_CL(list) == old_len - 1);
  CNode *node = pop_front_CL(list);
  free(node);
  node = pop_front_CL(list);
  free(node);
  clear_CL(list, free_cb, NULL);

  destroy_CL(list, free_cb, NULL);
}

void test_insert_erase_prev(void) {
  CList *list = newCList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }

  Item *newHead = newItem(-1);
  insert_CL(list, NULL, (CNode *)newHead);
  TEST_CHECK(size_CL(list) == 6);
  TEST_CHECK(front_CL(list) == (CNode *)newHead);
  TEST_CHECK(back_CL(list)->next == front_CL(list));
  TEST_CHECK(check_circular(list) == true);

  Item *newTail = newItem(100);
  insert_CL(list, list->tail, (CNode *)newTail);
  TEST_CHECK(size_CL(list) == 7);
  TEST_CHECK(back_CL(list) == (CNode *)newTail);
  TEST_CHECK(back_CL(list)->next == front_CL(list));
  TEST_CHECK(check_circular(list) == true);

  CNode *pre = find_CL(list, 2);
  Item *mid = newItem(50);
  insert_CL(list, pre, (CNode *)mid);
  TEST_CHECK(size_CL(list) == 8);
  CNode *cur = list->head;
  int expected[] = {-1, 0, 1, 50, 2, 3, 4, 100};
  for (int i = 0; i < 8; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular(list) == true);

  CNode *del = erase_CL(list, find_CL(list, 0));
  free(del);
  TEST_CHECK(size_CL(list) == 7);
  cur = list->head;
  int expected2[] = {-1, 50, 2, 3, 4, 100};
  clear_CL(list, free_cb, NULL);

  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }
  del = erase_CL(list, find_CL(list, 1));
  free(del);
  TEST_CHECK(size_CL(list) == 4);
  cur = list->head;
  int expected3[] = {
      0, 1, 3, 4};
                  
  int expected3_orig[] = {0, 1, 3,
                          4};
  for (int i = 0; i < 4; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected3_orig[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular(list) == true);

  del = erase_CL(list, NULL);
  free(del);
  TEST_CHECK(size_CL(list) == 3);
  TEST_CHECK(((Item *)front_CL(list))->val == 1);

  pre = find_CL(list, size_CL(list) - 2);
  del = erase_CL(list, pre);
  free(del);
  TEST_CHECK(size_CL(list) == 2);
  TEST_CHECK(((Item *)back_CL(list))->val == 3);

  del = erase_CL(list, back_CL(list));
  TEST_CHECK(del == NULL);
  TEST_CHECK(size_CL(list) == 2);

  destroy_CL(list, free_cb, NULL);
}

void test_reverse(void) {
  CList *list = newCList();

  reverse_CL(list);
  TEST_CHECK(size_CL(list) == 0);

  Item *single = newItem(1);
  push_back_CL(list, (CNode *)single);
  reverse_CL(list);
  TEST_CHECK(size_CL(list) == 1);
  TEST_CHECK(front_CL(list) == (CNode *)single);
  TEST_CHECK(back_CL(list) == (CNode *)single);
  TEST_CHECK(check_circular(list) == true);

  clear_CL(list, free_cb, NULL);

  for (int i = 0; i < 10; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }
  reverse_CL(list);
  CNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular(list) == true);

  TEST_CHECK(((Item *)front_CL(list))->val == 9);
  TEST_CHECK(((Item *)back_CL(list))->val == 0);
  TEST_CHECK(back_CL(list)->next == front_CL(list));

  reverse_CL(list);
  cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular(list) == true);

  destroy_CL(list, free_cb, NULL);
}

void test_clear_destroy(void) {
  CList *list = newCList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }
  TEST_CHECK(size_CL(list) == 5);

  clear_CL(list, free_cb, NULL);
  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  clear_CL(list, free_cb, NULL);

  destroy_CL(list, free_cb, NULL);

  list = newCList();
  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }
  clear_CL(list, NULL, NULL);
  TEST_CHECK(size_CL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  free(list);

  list = newCList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }
  destroy_CL(list, free_cb, NULL);
}

void test_mixed_operations(void) {
  CList *list = newCList();

  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i * 2);
    push_back_CL(list, (CNode *)it);
  }
  Item *it100 = newItem(100);
  insert_by_pos_CL(list, 2, (CNode *)it100);

  CNode *del = erase_by_pos_CL(list, 3);
  free(del);

  Item *neg = newItem(-1);
  push_front_CL(list, (CNode *)neg);

  CNode *popped = pop_back_CL(list);
  TEST_CHECK(((Item *)popped)->val == 8);
  free(popped);

  popped = pop_front_CL(list);
  TEST_CHECK(((Item *)popped)->val == -1);
  free(popped);

  reverse_CL(list);

  int expected[] = {6, 100, 2, 0};
  CNode *cur = list->head;
  for (int i = 0; i < 4; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == list->head);
  TEST_CHECK(check_circular(list) == true);

  Item *mid = newItem(50);
  insert_by_pos_CL(list, 2, (CNode *)mid);
  TEST_CHECK(size_CL(list) == 5);

  clear_CL(list, free_cb, NULL);
  TEST_CHECK(size_CL(list) == 0);

  destroy_CL(list, free_cb, NULL);
}

void test_error_handling(void) {
  clear_CL(NULL, free_cb, NULL);
  destroy_CL(NULL, free_cb, NULL);
  size_CL(NULL);
  empty_CL(NULL);
  front_CL(NULL);
  back_CL(NULL);
  push_back_CL(NULL, (CNode *)newItem(1));
  push_front_CL(NULL, (CNode *)newItem(1));
  pop_back_CL(NULL);
  pop_front_CL(NULL);
  find_CL(NULL, 0);
  insert_CL(NULL, NULL, (CNode *)newItem(1));
  erase_CL(NULL, NULL);
  insert_by_pos_CL(NULL, 0, (CNode *)newItem(1));
  erase_by_pos_CL(NULL, 0);
  reverse_CL(NULL);

  CList *list = newCList();
  push_back_CL(list, NULL);
  TEST_CHECK(size_CL(list) == 0);
  push_front_CL(list, NULL);
  TEST_CHECK(size_CL(list) == 0);
  insert_CL(list, NULL, NULL);
  TEST_CHECK(size_CL(list) == 0);
  insert_by_pos_CL(list, 0, NULL);
  TEST_CHECK(size_CL(list) == 0);
  erase_CL(list, NULL);
  TEST_CHECK(size_CL(list) == 0);

  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_CL(list, (CNode *)it);
  }
  CNode *del = erase_CL(list, NULL);
  free(del);
  TEST_CHECK(size_CL(list) == 2);
  TEST_CHECK(((Item *)front_CL(list))->val == 1);

  destroy_CL(list, free_cb, NULL);
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
