#include "list/DList.h"
#include "acutest.h"
#include <stdlib.h>

typedef struct Item {
  DNode node;
  int val;
} Item;

Item *newItem(int val) {
  Item *it = malloc(sizeof(Item));
  it->node.next = NULL;
  it->node.previous = NULL;
  it->val = val;
  return it;
}

void free_cb(DNode *node, void *args) {
  (void)args;
  free((Item *)node);
}

bool isequal(DNode *a, DNode *b) {
  if (!a || !b)
    return false;
  Item *ia = (Item *)a, *ib = (Item *)b;
  return ia->val == ib->val;
}

static bool check_prev_links(DList *list) {
  if (list->head == NULL)
    return list->tail == NULL;
  DNode *cur = list->head;
  while (cur->next) {
    if (cur->next->previous != cur)
      return false;
    cur = cur->next;
  }
  return cur == list->tail;
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

  clear_DL(list, free_cb, NULL);
  destroy_DL(list, free_cb, NULL);

  list = newDList();
  destroy_DL(list, free_cb, NULL);
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

  DNode *cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    if (i > 0) {
      TEST_CHECK(cur->previous == (DNode *)items[i - 1]);
    }
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  clear_DL(list, free_cb, NULL);
  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  destroy_DL(list, free_cb, NULL);
}

void test_push_front(void) {
  DList *list = newDList();
  Item *items[10];
  for (int i = 0; i < 10; ++i) {
    items[i] = newItem(i + 1);
    push_front_DL(list, (DNode *)items[i]);
  }

  TEST_CHECK(size_DL(list) == 10);
  TEST_CHECK(front_DL(list) == (DNode *)items[9]);
  TEST_CHECK(back_DL(list) == (DNode *)items[0]);

  DNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i + 1);
    if (i < 9) {
      TEST_CHECK(cur->previous == (DNode *)items[i + 1]);
    }
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  destroy_DL(list, free_cb, NULL);
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
    if (list->tail) {
      TEST_CHECK(list->tail->next == NULL);
    }
    TEST_CHECK(check_prev_links(list) == true);
  }

  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  TEST_CHECK(pop_back_DL(list) == NULL);

  destroy_DL(list, free_cb, NULL);
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
    if (list->head) {
      TEST_CHECK(list->head->previous == NULL);
    }
    TEST_CHECK(check_prev_links(list) == true);
  }

  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(empty_DL(list) == true);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  TEST_CHECK(pop_front_DL(list) == NULL);

  destroy_DL(list, free_cb, NULL);
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
  for (int i = 0; i < 89; ++i)
    cur = cur->next;
  TEST_CHECK(isequal(f, cur));

  TEST_CHECK(isequal(find_DL(list, 0), front_DL(list)));
  TEST_CHECK(isequal(find_DL(list, 99), back_DL(list)));

  TEST_CHECK(find_DL(list, 100) == NULL);
  TEST_CHECK(find_DL(list, -1) == NULL);
  TEST_CHECK(find_DL(list, 200) == NULL);

  clear_DL(list, free_cb, NULL);
  TEST_CHECK(find_DL(list, 0) == NULL);

  destroy_DL(list, free_cb, NULL);
}

void test_insert_by_pos(void) {
  DList *list = newDList();

  Item *it0 = newItem(100);
  insert_by_pos_DL(list, 0, (DNode *)it0);
  TEST_CHECK(size_DL(list) == 1);
  TEST_CHECK(front_DL(list) == (DNode *)it0);
  TEST_CHECK(back_DL(list) == (DNode *)it0);
  TEST_CHECK(check_prev_links(list) == true);

  Item *it1 = newItem(200);
  insert_by_pos_DL(list, 0, (DNode *)it1);
  TEST_CHECK(size_DL(list) == 2);
  TEST_CHECK(front_DL(list) == (DNode *)it1);
  TEST_CHECK(front_DL(list)->next == (DNode *)it0);
  TEST_CHECK(front_DL(list)->previous == NULL);
  TEST_CHECK(back_DL(list)->previous == front_DL(list));
  TEST_CHECK(check_prev_links(list) == true);

  Item *it2 = newItem(300);
  insert_by_pos_DL(list, size_DL(list), (DNode *)it2);
  TEST_CHECK(size_DL(list) == 3);
  TEST_CHECK(back_DL(list) == (DNode *)it2);
  TEST_CHECK(back_DL(list)->previous == (DNode *)it0);
  TEST_CHECK(check_prev_links(list) == true);

  Item *it3 = newItem(250);
  insert_by_pos_DL(list, 1, (DNode *)it3);
  TEST_CHECK(size_DL(list) == 4);
  DNode *cur = list->head;
  TEST_CHECK(((Item *)cur)->val == 200);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 250);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 100);
  cur = cur->next;
  TEST_CHECK(((Item *)cur)->val == 300);
  cur = cur->next;
  TEST_CHECK(cur == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  int old = size_DL(list);
  insert_by_pos_DL(list, -1, (DNode *)newItem(999));
  TEST_CHECK(size_DL(list) == old);
  insert_by_pos_DL(list, old + 5, (DNode *)newItem(999));
  TEST_CHECK(size_DL(list) == old);
  insert_by_pos_DL(list, 0, NULL);
  TEST_CHECK(size_DL(list) == old);

  destroy_DL(list, free_cb, NULL);
}

void test_erase_by_pos(void) {
  DList *list = newDList();
  for (int i = 0; i < 5; ++i) {
    Item *item = newItem(i);
    push_back_DL(list, (DNode *)item);
  }

  DNode *del = erase_by_pos_DL(list, 0);
  free(del);
  TEST_CHECK(size_DL(list) == 4);
  TEST_CHECK(((Item *)front_DL(list))->val == 1);
  TEST_CHECK(front_DL(list)->previous == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  del = erase_by_pos_DL(list, size_DL(list) - 1);
  free(del);
  TEST_CHECK(size_DL(list) == 3);
  TEST_CHECK(((Item *)back_DL(list))->val == 3);
  TEST_CHECK(back_DL(list)->next == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  del = erase_by_pos_DL(list, 1);
  free(del);
  TEST_CHECK(size_DL(list) == 2);
  TEST_CHECK(((Item *)front_DL(list))->val == 1);
  TEST_CHECK(((Item *)back_DL(list))->val == 3);
  TEST_CHECK(check_prev_links(list) == true);

  del = erase_by_pos_DL(list, 0);
  free(del);
  TEST_CHECK(size_DL(list) == 1);
  TEST_CHECK(((Item *)front_DL(list))->val == 3);

  del = erase_by_pos_DL(list, 0);
  free(del);
  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  erase_by_pos_DL(list, 0);
  TEST_CHECK(size_DL(list) == 0);
  erase_by_pos_DL(list, -1);
  TEST_CHECK(size_DL(list) == 0);
  erase_by_pos_DL(list, 5);
  TEST_CHECK(size_DL(list) == 0);

  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_DL(list, (DNode *)it);
  }
  int old_len = size_DL(list);
  erase_by_pos_DL(list, 1);
  TEST_CHECK(size_DL(list) == old_len - 1);
  DNode *node = pop_front_DL(list);
  free(node);
  node = pop_front_DL(list);
  free(node);
  clear_DL(list, free_cb, NULL);

  destroy_DL(list, free_cb, NULL);
}

void test_insert_erase_prev(void) {
  DList *list = newDList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_DL(list, (DNode *)it);
  }

  Item *newHead = newItem(-1);
  insert_DL(list, NULL, (DNode *)newHead);
  TEST_CHECK(size_DL(list) == 6);
  TEST_CHECK(front_DL(list) == (DNode *)newHead);
  TEST_CHECK(((DNode *)newHead)->previous == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  Item *newTail = newItem(100);
  insert_DL(list, list->tail, (DNode *)newTail);
  TEST_CHECK(size_DL(list) == 7);
  TEST_CHECK(back_DL(list) == (DNode *)newTail);
  TEST_CHECK(((DNode *)newTail)->next == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  DNode *pre = find_DL(list, 2);
  Item *mid = newItem(50);
  insert_DL(list, pre, (DNode *)mid);
  TEST_CHECK(size_DL(list) == 8);
  DNode *cur = list->head;
  int expected[] = {-1, 0, 1, 50, 2, 3, 4, 100};
  for (int i = 0; i < 8; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }
  TEST_CHECK(check_prev_links(list) == true);

  DNode *del = erase_DL(list, find_DL(list, 1));
  free(del);
  TEST_CHECK(size_DL(list) == 7);
  cur = list->head;
  int expected2[] = {-1, 1, 50, 2, 3, 4, 100};
  for (int i = 0; i < 7; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected2[i]);
    cur = cur->next;
  }
  TEST_CHECK(check_prev_links(list) == true);

  del = erase_DL(list, front_DL(list));
  free(del);
  TEST_CHECK(size_DL(list) == 6);
  TEST_CHECK(((Item *)front_DL(list))->val == 1); // 现在头部应该是1
  TEST_CHECK(front_DL(list)->previous == NULL);

  del = erase_DL(list, back_DL(list));
  free(del);
  TEST_CHECK(size_DL(list) == 5);
  TEST_CHECK(((Item *)back_DL(list))->val == 4);
  TEST_CHECK(back_DL(list)->next == NULL);

  del = erase_DL(list, NULL);
  TEST_CHECK(del == NULL);
  TEST_CHECK(size_DL(list) == 5);

  destroy_DL(list, free_cb, NULL);
}

void test_reverse(void) {
  DList *list = newDList();

  reverse_DL(list);
  TEST_CHECK(size_DL(list) == 0);

  Item *single = newItem(1);
  push_back_DL(list, (DNode *)single);
  reverse_DL(list);
  TEST_CHECK(size_DL(list) == 1);
  TEST_CHECK(front_DL(list) == (DNode *)single);
  TEST_CHECK(back_DL(list) == (DNode *)single);

  clear_DL(list, free_cb, NULL);

  for (int i = 0; i < 10; ++i) {
    Item *it = newItem(i);
    push_back_DL(list, (DNode *)it);
  }
  reverse_DL(list);
  DNode *cur = list->head;
  for (int i = 9; i >= 0; --i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  TEST_CHECK(((Item *)front_DL(list))->val == 9);
  TEST_CHECK(((Item *)back_DL(list))->val == 0);

  reverse_DL(list);
  cur = list->head;
  for (int i = 0; i < 10; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == i);
    cur = cur->next;
  }
  TEST_CHECK(check_prev_links(list) == true);

  destroy_DL(list, free_cb, NULL);
}

void test_clear_destroy(void) {
  DList *list = newDList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_DL(list, (DNode *)it);
  }
  TEST_CHECK(size_DL(list) == 5);

  clear_DL(list, free_cb, NULL);
  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);

  clear_DL(list, free_cb, NULL);

  destroy_DL(list, free_cb, NULL);

  list = newDList();
  for (int i = 0; i < 3; ++i) {
    Item *it = newItem(i);
    push_back_DL(list, (DNode *)it);
  }
  clear_DL(list, NULL, NULL);
  TEST_CHECK(size_DL(list) == 0);
  TEST_CHECK(list->head == NULL);
  TEST_CHECK(list->tail == NULL);
  free(list);

  list = newDList();
  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i);
    push_back_DL(list, (DNode *)it);
  }
  destroy_DL(list, free_cb, NULL);
}

void test_mixed_operations(void) {
  DList *list = newDList();

  for (int i = 0; i < 5; ++i) {
    Item *it = newItem(i * 2);
    push_back_DL(list, (DNode *)it);
  }
  Item *it100 = newItem(100);
  insert_by_pos_DL(list, 2, (DNode *)it100);

  DNode *del = erase_by_pos_DL(list, 3);
  free(del);

  Item *neg = newItem(-1);
  push_front_DL(list, (DNode *)neg);

  DNode *popped = pop_back_DL(list);
  TEST_CHECK(((Item *)popped)->val == 8);
  free(popped);

  popped = pop_front_DL(list);
  TEST_CHECK(((Item *)popped)->val == -1);
  free(popped);

  reverse_DL(list);

  int expected[] = {6, 100, 2, 0};
  DNode *cur = list->head;
  for (int i = 0; i < 4; ++i) {
    Item *it = (Item *)cur;
    TEST_CHECK(it->val == expected[i]);
    cur = cur->next;
  }
  TEST_CHECK(cur == NULL);
  TEST_CHECK(check_prev_links(list) == true);

  Item *mid = newItem(50);
  insert_by_pos_DL(list, 2, (DNode *)mid);
  TEST_CHECK(size_DL(list) == 5);

  clear_DL(list, free_cb, NULL);
  TEST_CHECK(size_DL(list) == 0);

  destroy_DL(list, free_cb, NULL);
}

void test_error_handling(void) {
  clear_DL(NULL, free_cb, NULL);
  destroy_DL(NULL, free_cb, NULL);
  size_DL(NULL);
  empty_DL(NULL);
  front_DL(NULL);
  back_DL(NULL);
  push_back_DL(NULL, (DNode *)newItem(1));
  push_front_DL(NULL, (DNode *)newItem(1));
  pop_back_DL(NULL);
  pop_front_DL(NULL);
  find_DL(NULL, 0);
  insert_DL(NULL, NULL, (DNode *)newItem(1));
  erase_DL(NULL, NULL); // 移除 free_cb 参数
  insert_by_pos_DL(NULL, 0, (DNode *)newItem(1));
  erase_by_pos_DL(NULL, 0);
  reverse_DL(NULL);

  DList *list = newDList();
  push_back_DL(list, NULL);
  TEST_CHECK(size_DL(list) == 0);
  push_front_DL(list, NULL);
  TEST_CHECK(size_DL(list) == 0);
  insert_DL(list, NULL, NULL);
  TEST_CHECK(size_DL(list) == 0);
  insert_by_pos_DL(list, 0, NULL);
  TEST_CHECK(size_DL(list) == 0);
  DNode *res = erase_DL(list, NULL);
  TEST_CHECK(res == NULL);
  TEST_CHECK(size_DL(list) == 0);

  destroy_DL(list, free_cb, NULL);
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
