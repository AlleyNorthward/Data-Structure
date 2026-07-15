#include "list/List.h"
#include <stdlib.h>

static void clear(List *list, void (*free_cb)(Node *node, void *args),
                  void *args) {
  if (list == NULL || list->head == NULL)
    return;

  Node *cur = list->head;
  while (cur) {
    Node *del = cur;
    cur = cur->next;
    if (free_cb)
      free_cb(del, args);
  }

  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}

static Node *find(List *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len)
    return NULL;

  Node *cur = list->head;
  for (int i = 0; i < pos; ++i) {
    cur = cur->next;
  }

  return cur;
}

static Node *cut_node(List *list, Node *pre) {
  if (list == NULL || list->len == 0 || pre == list->tail)
    return NULL;

  Node *del = NULL;

  if (list->head == list->tail) {
    del = list->head;
    del->next = NULL;
    list->head = NULL;
    list->tail = NULL;
    --list->len;
    return del;
  }

  if (pre == NULL) {
    del = list->head;
    list->head = list->head->next;
    del->next = NULL;
    --list->len;
    return del;
  }

  if (pre->next == list->tail) {
    del = pre->next;
    list->tail = pre;
    pre->next = NULL;
    del->next = NULL;
    --list->len;
    return del;
  }

  Node *next = pre->next;
  del = next;

  pre->next = del->next;
  del->next = NULL;

  --list->len;
  return del;
}

static void add_node(List *list, Node *pre, Node *node) {
  if (list == NULL || node == NULL)
    return;

  if (list->head == NULL) {
    list->head = node;
    list->tail = node;
    node->next = NULL;
    ++list->len;
    return;
  }

  if (pre == NULL) {
    node->next = list->head;
    list->head = node;
    ++list->len;
    return;
  }

  if (pre == list->tail) {
    list->tail->next = node;
    node->next = NULL;
    list->tail = node;
    ++list->len;
    return;
  }

  Node *next = pre->next;
  node->next = next;
  pre->next = node;

  ++list->len;
}

List *newList() {
  List *list = malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;

  return list;
}

bool empty_L(List *list) {
  if (list == NULL)
    return 0;
  return list->head == NULL;
}

void clear_L(List *list, void (*free_cb)(Node *node, void *args), void *args) {
  if (list == NULL)
    return;

  clear(list, free_cb, args);
}

void destroy_L(List *list, void (*free_cb)(Node *node, void *args),
               void *args) {
  if (list == NULL)
    return;
  clear(list, free_cb, args);
  free(list);
}

int size_L(List *list) {
  if (list == NULL)
    return 0;

  return list->len;
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

void push_back_L(List *list, Node *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, list->tail, node);
}

void push_front_L(List *list, Node *node) {
  if (list == NULL || node == NULL)
    return;
  add_node(list, NULL, node);
}

Node *pop_back_L(List *list) {
  if (list == NULL)
    return NULL;
  Node *pre = find(list, list->len - 2);
  return cut_node(list, pre);
}

Node *pop_front_L(List *list) {
  if (list == NULL)
    return NULL;

  return cut_node(list, NULL);
}

Node *find_L(List *list, int pos) { return find(list, pos); }

void insert_L(List *list, Node *pre, Node *node) { add_node(list, pre, node); }

Node *erase_L(List *list, Node *pre) { return cut_node(list, pre); }

void insert_by_pos_L(List *list, int pos, Node *node) {
  if (list == NULL || node == NULL || pos < 0 || pos > list->len)
    return;
  Node *pre = find(list, pos - 1);
  add_node(list, pre, node);
}

Node *erase_by_pos_L(List *list, int pos) {
  if (list == NULL || pos < 0 || pos >= list->len)
    return NULL;
  Node *pre = find(list, pos - 1);
  Node *del = cut_node(list, pre);

  return del;
}

void reverse_L(List *list) {
  if (list == NULL || list->len == 0 || list->len == 1)
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
