#include "CList.h"
#include <stdio.h>
#include <stdlib.h>

CList *newCList() {
  CList *list = (CList *)malloc(sizeof(CList));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}

bool empty_CL(CList *list) {
  if (list == NULL)
    return true;
  return list->head == NULL;
}

void clear_CL(CList *list, void (*free_cb)(CNode *node)) {
  if (list == NULL || empty_CL(list))
    return;

  CNode *cur = list->head;
  list->tail->next = NULL;

  while (cur != NULL) {
    CNode *del = cur;
    cur = cur->next;
    if (free_cb != NULL)
      free_cb(del);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void destroy_CL(CList *list, void (*free_cb)(CNode *node)) {
  if (list == NULL)
    return;
  clear_CL(list, free_cb);
  free(list);
}

int size_CL(CList *list) {
  if (list == NULL)
    return 0;

  return list->len;
}

CNode *front_CL(CList *list) {
  if (list == NULL)
    return NULL;
  return list->head;
}

CNode *back_CL(CList *list) {
  if (list == NULL)
    return NULL;
  return list->tail;
}

void print_CL(CList *list, void (*print_cb)(CNode *node)) {
  if (list == NULL || print_cb == NULL || empty_CL(list)) {
    putchar('\n');
    return;
  }

  CNode *cur = list->head;

  do {
    print_cb(cur);
    cur = cur->next;
  } while (cur != list->head);

  putchar('\n');
}

void push_back_CL(CList *list, CNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (empty_CL(list)) {
    list->head = node;
    node->next = node;
  } else {
    list->tail->next = node;
    node->next = list->head;
  }

  ++list->len;
  list->tail = node;
}

void push_front_CL(CList *list, CNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (empty_CL(list)) {
    list->tail = node;
    node->next = node;
  } else {
    node->next = list->head;
    list->tail->next = node;
  }

  ++list->len;
  list->head = node;
}

CNode *pop_back_CL(CList *list) {
  if (list == NULL || empty_CL(list))
    return NULL;

  CNode *del = list->tail;
  if (list->head == list->tail) {
    list->tail = NULL;
    list->head = NULL;
    --list->len;
    del->next = NULL;
    return del;
  }

  CNode *cur = list->head;
  while (cur->next != list->tail) {
    cur = cur->next;
  }

  cur->next = del->next;
  del->next = NULL;
  list->tail = cur;
  --list->len;
  return del;
}

CNode *pop_front_CL(CList *list) {
  if (list == NULL || empty_CL(list))
    return NULL;

  CNode *del = list->head;
  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
    --list->len;
    del->next = NULL;
    return del;
  }

  list->tail->next = del->next;
  del->next = NULL;
  list->head = list->tail->next;
  --list->len;
  return del;
}

void reverse_CL(CList *list) {
  if (list == NULL || empty_CL(list) || size_CL(list) == 1)
    return;

  CNode *cur = list->head;
  CNode *pre = list->tail;
  CNode *next = NULL;

  do {
    next = cur->next;
    cur->next = pre;
    pre = cur;
    cur = next;
  } while (cur != list->head);

  list->tail = cur;
  list->head = pre;
}

CNode *find_CL(CList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= size_CL(list))
    return NULL;

  CNode *cur = front_CL(list);
  for (int i = 0; i < pos; ++i) {
    cur = cur->next;
  }

  return cur;
}

void insert_CL(CList *list, CNode *pre, CNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (pre == NULL) {
    push_front_CL(list, node);
    return;
  }

  if (pre == back_CL(list)) {
    push_back_CL(list, node);
    return;
  }

  node->next = pre->next;
  pre->next = node;
  ++list->len;
}

void erase_CL(CList *list, CNode *pre, void (*free_cb)(CNode *node)) {
  if (list == NULL)
    return;

  CNode *del = NULL;
  if (pre == NULL) {
    del = pop_front_CL(list);
    if (free_cb)
      free_cb(del);
    return;
  }

  if (pre->next == back_CL(list)) {
    del = pop_back_CL(list);
    if (free_cb)
      free_cb(del);
    return;
  }

  del = pre->next;
  pre->next = del->next;
  if (free_cb)
    free_cb(del);
  --list->len;
}

void insert_by_pos_CL(CList *list, int pos, CNode *node) {
  if (list == NULL || node == NULL || pos < 0 || pos > size_CL(list))
    return;
  CNode *pre = find_CL(list, pos - 1);
  insert_CL(list, pre, node);
}

void erase_by_pos_CL(CList *list, int pos, void (*free_cb)(CNode *node)) {
  if (list == NULL || pos < 0 || pos >= size_CL(list))
    return;

  CNode *pre = find_CL(list, pos - 1);
  erase_CL(list, pre, free_cb);
}
