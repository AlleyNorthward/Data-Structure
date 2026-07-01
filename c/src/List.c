#include "List.h"
#include <stdio.h>
#include <stdlib.h>

List *newList() {
  List *list = (List *)malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}

void destory_L(List *list, void (*free_cb)(Node *node)) {
  clear(list, free_cb);
  free(list);
}

void push_back_L(List *list, Node *node) {
  if (empty_L(list)) {
    list->head = node;
  } else {
    list->tail->next = node;
  }
  list->tail = node;
  ++list->len;
}

Node *pop_back_L(List *list) {
  if (empty_L(list))
    return NULL;
  Node *del = list->tail;

  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
    --list->len;
    return del;
  }

  Node *cur = list->head;
  while (cur->next != list->tail) {
    cur = cur->next;
  }
  list->tail = cur;
  --list->len;
  return del;
}

void push_front_L(List *list, Node *node) {
  if (empty_L(list)) {
    list->tail = node;
  } else {
    node->next = list->head;
  }
  list->head = node;
  ++list->len;
}

Node *pop_front_L(List *list) {
  if (list->head == NULL)
    return NULL;

  Node *del = list->head;
  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
    --list->len;
    return del;
  }

  list->head = list->head->next;
  --list->len;
  return del;
}

Node *front_L(List *list) { return list->head; }

Node *back_L(List *list) { return list->tail; }

void print_L(List *list, void (*print_cb)(Node *node)) {
  Node *cur = list->head;
  while (cur) {
    print_cb(cur);
    cur = cur->next;
  }
  putchar('\n');
}

bool empty_L(List *list) { return list->head == NULL; }

int size_L(List *list) { return list->len; }

void clear(List *list, void (*free_cb)(Node *node)) {
  Node *cur = list->head;
  while (cur) {
    Node *del = cur;
    cur = cur->next;
    free_cb(del);
  }
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void insert_L(List *list, int pos, Node *node) {
  // 栈上pos合法区间 [0, size]
  if (node == NULL || pos < 0 || pos > size_L(list))
    return;

  if (pos == 0) {
    push_front_L(list, node);
    return;
  }

  if (pos == size_L(list)) {
    push_back_L(list, node);
    return;
  }

  Node *cur = list->head;
  for (int i = 0; i < pos - 1; ++i) {
    cur = cur->next;
  }

  node->next = cur->next;
  cur->next = node;
  ++list->len;
}

void erase_L(List *list, int pos, void (*free_cb)(Node *node)) {
  if (list == NULL || pos < 0 || pos > size_L(list) - 1 || free_cb == NULL) {
    return;
  }

  if (pos == 0) {
    Node *del = pop_front_L(list);
    free_cb(del);
    return;
  }

  if (pos == size_L(list) - 1) {
    Node *del = pop_back_L(list);
    free_cb(del);
    return;
  }

  Node *cur = list->head;
  for (int i = 0; i < pos - 1; ++i) {
    cur = cur->next;
  }

  Node *del = cur->next;
  cur->next = del->next;
  free_cb(del);
  --list->len;
}

void reverse_L(List *list) {
  if (list == NULL)
    return;

  Node *cur = list->head;
  Node *prev = NULL;
  Node *next = NULL;

  while (cur) {
    next = cur->next;
    cur->next = prev;
    prev = cur;
    cur = next;
  }

  Node *old_head = list->head;
  list->head = prev;
  list->tail = old_head;
}
