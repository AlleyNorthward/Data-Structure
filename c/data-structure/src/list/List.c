#include "List.h"
#include <stdio.h>
#include <stdlib.h>

List *newList() {
  List *list = (List *)malloc(sizeof(List));
  if (list == NULL) {
    return NULL;
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
  return list;
}

bool empty_L(List *list) {
  if (list == NULL)
    return true;

  return list->head == NULL;
}

void clear_L(List *list, void (*free_cb)(Node *node)) {
  if (list == NULL)
    return;

  Node *cur = list->head;
  while (cur) {
    Node *del = cur;
    cur = cur->next;
    if (free_cb != NULL) {
      free_cb(del);
    }
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

void destroy_L(List *list, void (*free_cb)(Node *node)) {
  if (list == NULL)
    return;

  clear_L(list, free_cb);
  free(list);
}

void push_back_L(List *list, Node *node) {
  if (list == NULL || node == NULL)
    return;

  node->next = NULL;
  if (empty_L(list)) {
    list->head = node;
  } else {
    list->tail->next = node;
  }

  list->tail = node;
  ++list->len;
}

void push_front_L(List *list, Node *node) {
  if (list == NULL || node == NULL)
    return;
  node->next = list->head;

  if (empty_L(list)) {
    list->tail = node;
  }
  list->head = node;
  ++list->len;
}

Node *pop_back_L(List *list) {
  if (list == NULL || empty_L(list))
    return NULL;

  Node *del = list->tail;
  if (list->tail == list->head) {
    del->next = NULL;
    list->head = NULL;
    list->tail = NULL;
    --list->len;
    return del;
  }

  Node *cur = list->head;
  while (cur->next != list->tail) {
    cur = cur->next;
  }
  cur->next = NULL;
  list->tail = cur;
  del->next = NULL;
  --list->len;
  return del;
}

Node *pop_front_L(List *list) {
  if (list == NULL || empty_L(list))
    return NULL;

  Node *del = list->head;
  if (list->head == list->tail) {
    del->next = NULL;
    list->tail = NULL;
    list->head = NULL;
    --list->len;
    return del;
  }

  list->head = del->next;
  del->next = NULL;
  --list->len;

  return del;
}

Node *front_L(List *list) {
  if (list == NULL)
    return NULL;
  return list->head;
}

Node *back_L(List *list) {
  if (list == NULL)
    return NULL;

  return list->tail;
}

int size_L(List *list) {
  if (list == NULL)
    return 0;
  return list->len;
}

void print_L(List *list, void (*print_cb)(Node *node)) {
  if (list == NULL || print_cb == NULL || empty_L(list)) {
    putchar('\n');
    return;
  }

  Node *cur = list->head;

  while (cur) {
    print_cb(cur);
    cur = cur->next;
  }

  putchar('\n');
}

Node *find_L(List *list, int pos) {
  // pos [0, size);
  if (list == NULL || pos < 0 || pos >= size_L(list))
    return NULL;

  Node *cur = list->head;

  for (int i = 0; i < pos; ++i) {
    cur = cur->next;
  }

  return cur;
}

void insert_by_pos_L(List *list, int pos, Node *node) {
  if (list == NULL || node == NULL || pos < 0 || pos > size_L(list))
    return;

  Node *pre = find_L(list, pos - 1);
  insert_L(list, pre, node);
}

void erase_by_pos_L(List *list, int pos, void (*free_cb)(Node *node)) {
  if (list == NULL || pos < 0 || pos >= size_L(list))
    return;

  Node *pre = find_L(list, pos - 1);
  erase_L(list, pre, free_cb);
}

void reverse_L(List *list) {
  if (list == NULL)
    return;

  if (list->head == list->tail)
    return;

  Node *cur = list->head;
  Node *pre = NULL;
  Node *next = NULL;

  while (cur) {
    next = cur->next;
    cur->next = pre;
    pre = cur;
    cur = next;
  }

  list->tail = list->head;
  list->head = pre;
}

void insert_L(List *list, Node *pre, Node *node) {
  if (list == NULL || node == NULL)
    return;

  if (pre == NULL) {
    push_front_L(list, node);
    return;
  }

  if (pre == list->tail) {
    push_back_L(list, node);
    return;
  }

  Node *next = pre->next;
  node->next = next;
  pre->next = node;
  ++list->len;
}

void erase_L(List *list, Node *pre, void (*free_cb)(Node *node)) {
  if (list == NULL)
    return;

  Node *del = NULL;
  if (pre == NULL) {
    del = pop_front_L(list);
    if (free_cb)
      free_cb(del);
    return;
  }

  if (pre->next == back_L(list)) {
    del = pop_back_L(list);
    if (free_cb)
      free_cb(del);
    return;
  }

  del = pre->next;
  pre->next = del->next;
  del->next = NULL;
  if (free_cb)
    free_cb(del);
  --list->len;
}
