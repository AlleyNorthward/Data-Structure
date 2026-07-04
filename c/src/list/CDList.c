#include "CDList.h"
#include <stdio.h>
#include <stdlib.h>

CDList *newCDList() {
  CDList *list = (CDList *)malloc(sizeof(CDList));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}

bool empty_CDL(CDList *list) {
  if (list == NULL)
    return true;

  return list->head == NULL;
}

void clear_CDL(CDList *list, void (*free_cb)(CDNode *node)) {
  if (list == NULL || empty_CDL(list))
    return;

  list->tail->next = NULL;
  list->head->previous = NULL;
  CDNode *cur = list->head;
  while (cur != NULL) {
    CDNode *del = cur;
    cur = cur->next;
    if (free_cb != NULL)
      free_cb(del);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void destroy_CDL(CDList *list, void (*free_cb)(CDNode *node)) {
  if (list == NULL)
    return;
  clear_CDL(list, free_cb);
  free(list);
}

int size_CDL(CDList *list) {
  if (list == NULL)
    return 0;

  return list->len;
}

CDNode *front_CDL(CDList *list) {
  if (list == NULL)
    return NULL;

  return list->head;
}

CDNode *back_CDL(CDList *list) {
  if (list == NULL)
    return NULL;

  return list->tail;
}

void print_CDL(CDList *list, void (*print_cb)(CDNode *node)) {
  if (list == NULL || print_cb == NULL || empty_CDL(list)) {
    putchar('\n');
    return;
  }

  CDNode *cur = list->head;
  do {
    print_cb(cur);
    cur = cur->next;
  } while (cur != list->head);

  putchar('\n');
}

void push_back_CDL(CDList *list, CDNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (empty_CDL(list)) {
    list->head = node;
    node->next = node;
    node->previous = node;
  } else {
    node->next = list->head;
    node->previous = list->tail;

    list->tail->next = node;
    list->head->previous = node;
  }

  ++list->len;
  list->tail = node;
}

void push_front_CDL(CDList *list, CDNode *node) {
  if (list == NULL || node == NULL)
    return;

  if (empty_CDL(list)) {
    list->tail = node;
    node->next = node;
    node->previous = node;
  } else {
    node->next = list->head;
    node->previous = list->tail;
    list->tail->next = node;
    list->head->previous = node;
  }

  ++list->len;
  list->head = node;
}

CDNode *pop_back_CDL(CDList *list) {
  if (list == NULL || empty_CDL(list))
    return NULL;

  CDNode *del = list->tail;
  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
    --list->len;
    del->next = NULL;
    del->previous = NULL;
    return del;
  }

  CDNode *pre = del->previous;
  CDNode *next = del->next;

  pre->next = next;
  next->previous = pre;
  list->tail = pre;
  --list->len;

  del->next = NULL;
  del->previous = NULL;

  return del;
}

CDNode *pop_front_CDL(CDList *list) {
  if (list == NULL || empty_CDL(list))
    return NULL;

  CDNode *del = list->head;
  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
    --list->len;
    del->next = NULL;
    del->previous = NULL;
    return del;
  }

  CDNode *pre = del->previous;
  CDNode *next = del->next;

  pre->next = next;
  next->previous = pre;
  list->head = next;
  --list->len;

  del->next = NULL;
  del->previous = NULL;
  return del;
}

void insert_CDL(CDList *list, int pos, CDNode *node) {
  if (list == NULL || node == NULL || pos < 0 || pos > size_CDL(list))
    return;

  if (pos == 0) {
    push_front_CDL(list, node);
    return;
  }

  if (pos == size_CDL(list)) {
    push_back_CDL(list, node);
    return;
  }

  CDNode *cur = list->head;
  for (int i = 0; i < pos - 1; ++i) {
    cur = cur->next;
  }

  node->next = cur->next;
  node->previous = cur;
  cur->next = node;
  cur->next->previous = node;
  ++list->len;
}

void erase_CDL(CDList *list, int pos, void (*free_cb)(CDNode *node)) {
  if (list == NULL || empty_CDL(list) || pos < 0 || pos > size_CDL(list) - 1)
    return;

  if (pos == 0) {
    CDNode *node = pop_front_CDL(list);
    if (free_cb != NULL)
      free_cb(node);
    return;
  }

  if (pos == size_CDL(list) - 1) {
    CDNode *node = pop_back_CDL(list);
    if (free_cb != NULL)
      free_cb(node);

    return;
  }

  CDNode *cur = list->head;
  for (int i = 0; i < pos - 1; ++i) {
    cur = cur->next;
  }

  CDNode *del = cur->next;
  CDNode *next = del->next;
  cur->next = next;
  next->previous = cur;

  del->next = NULL;
  del->previous = NULL;
  --list->len;

  if (!free_cb)
    free_cb(del);
}

void reverse_CDL(CDList *list) {
  if (list == NULL || empty_CDL(list))
    return;

  if (size_CDL(list) == 1)
    return;

  CDNode *cur = list->head;
  CDNode *pre = list->tail;
  CDNode *next = NULL;

  do {
    next = cur->next;
    cur->next = pre;
    cur->previous = next;
    pre = cur;
    cur = next;
  } while (cur != list->head);

  list->tail = cur;
  list->head = pre;
}
