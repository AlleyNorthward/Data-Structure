#include "List.h"
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
}

bool empty_L(List *list) { return list->head == NULL; }

int size_L(List *list) { return list->len; }

void clear(List *list, void (*free_cb)(Node *node)) {
  Node *cur = list->head;
  while (cur) {
    Node *del = cur;
    cur = cur->next;
    free_cb(cur);
  }
  list->head = NULL;
  list->tail = NULL;
}

void insert_L(List *list, int pos, Node *node) {
  // 栈上pos合法区间 [0, size]
  if (node == NULL || pos < 0 || pos > size_L(list))
    return;
}
