#include "IList.h"
#include <stdlib.h>
#include <string.h>

static void update_array(IList *list, Node *node, int (*key_cb)(Node *node),
                         Node *val) {
  int key = key_cb(node);
  if (key >= 0 && key <= list->max_key) {
    Node **slot = (Node **)at_DA(list->array, key);
    if (slot)
      *slot = val;
  }
}

IList *newIList(int max_key_) {
  IList *list = malloc(sizeof(IList));
  if (list == NULL)
    return NULL;

  list->list = newList();
  if (list->list == NULL) {
    free(list);
    return NULL;
  }

  list->array = newDArray(max_key_ + 1, sizeof(Node *));
  if (list->array == NULL) {
    free(list->list);
    free(list);
    return NULL;
  }

  list->array->len = max_key_ + 1;
  list->max_key = max_key_;
  return list;
}

bool empty_IL(IList *list) {
  if (list == NULL)
    return true;
  return empty_L(list->list);
}

void clear_IL(IList *list, void (*free_cb)(Node *node)) {
  if (list == NULL || empty_IL(list))
    return;

  clear_L(list->list, free_cb);
  int size = list->max_key + 1;
  int total_bytes = size * list->array->elem_size;
  memset(list->array->data, 0, total_bytes);
}

void destroy_IL(IList *list, void (*free_cb)(Node *node)) {
  if (list == NULL)
    return;
  destroy_L(list->list, free_cb);
  destroy_DA(list->array, NULL);
  free(list);
}

int size_IL(IList *list) {
  if (list == NULL || empty_IL(list))
    return 0;
  return size_L(list->list);
}

Node *front_IL(IList *list) {
  if (list == NULL)
    return NULL;
  return front_L(list->list);
}

Node *back_IL(IList *list) {
  if (list == NULL)
    return NULL;
  return back_L(list->list);
}

void print_IL(IList *list, void (*print_cb)(Node *node)) {
  if (list == NULL || print_cb == NULL)
    return;

  print_L(list->list, print_cb);
}

void push_back_IL(IList *list, Node *node, int (*key_cb)(Node *node)) {
  if (list == NULL)
    return;
  push_back_L(list->list, node);
  update_array(list, node, key_cb, node);
}

void push_front_IL(IList *list, Node *node, int (*key_cb)(Node *node)) {
  if (list == NULL)
    return;
  push_front_L(list->list, node);
  update_array(list, node, key_cb, node);
}

Node *pop_back_IL(IList *list, int (*key_cb)(Node *node)) {
  if (list == NULL)
    return NULL;

  Node *n = pop_back_L(list->list);
  if (n == NULL)
    return NULL;
  update_array(list, n, key_cb, NULL);
  return n;
}

Node *pop_front_IL(IList *list, int (*key_cb)(Node *node)) {
  if (list == NULL)
    return NULL;
  Node *n = pop_front_L(list->list);
  if (n == NULL)
    return NULL;
  update_array(list, n, key_cb, NULL);
  return n;
}

void insert_IL(IList *list, Node *node, int pos, int (*key_cb)(Node *node)) {
  if (list == NULL || pos < 0 || pos > size_IL(list) || key_cb == NULL) {
    return;
  }

  // base0
  if (pos == 0) {
    push_front_IL(list, node, key_cb);
    return;
  }

  if (pos == size_IL(list)) {
    push_back_IL(list, node, key_cb);
    return;
  }

  Node *cur = *(Node **)at_DA(list->array, pos - 1);
  node->next = cur->next;
  cur->next = node;

  ++list->list->len;
  update_array(list, node, key_cb, node);
}

void erase_IL(IList *list, int pos, void (*free_cb)(Node *node),
              int (*key_cb)(Node *node)) {
  // pos [0, size)
  if (list == NULL || pos < 0 || pos >= size_IL(list))
    return;

  if (pos == 0) {
    pop_front_IL(list, key_cb);
    return;
  }

  if (pos == size_IL(list) - 1) {
    pop_back_IL(list, key_cb);
    return;
  }

  Node *pre = *(Node **)at_DA(list->array, pos - 1);
  Node *del = pre->next;
  pre->next = del->next;
  del->next = NULL;

  --list->list->len;
  update_array(list, del, key_cb, NULL);
  free_cb(del);
}
