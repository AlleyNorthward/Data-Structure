#include "IList.h"
#include <stdlib.h>
#include <string.h>

static void update_array(IList *list, CDNode *node,
                         int (*get_val)(CDNode *node), CDNode *val) {
  int value = get_val(node);
  if (value >= 0 && value <= list->max_key) {
    CDNode **slot = (CDNode **)at_DA(list->array, value);
    if (slot)
      *slot = val;
  }
}

static bool isequal(CDNode *a, CDNode *b, int (*get_val)(CDNode *node)) {
  if (!a || !b)
    return NULL;

  return get_val(a) == get_val(b);
}

IList *newIList(int max_key_) {
  IList *list = malloc(sizeof(IList));
  if (list == NULL)
    return NULL;

  list->list = newCDList();
  if (list->list == NULL) {
    free(list);
    return NULL;
  }

  list->array = newDArray(max_key_ + 1, sizeof(CDNode *));
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
  return empty_CDL(list->list);
}

void clear_IL(IList *list, void (*free_cb)(CDNode *node)) {
  if (list == NULL || empty_IL(list))
    return;

  clear_CDL(list->list, free_cb);
  int size = list->max_key + 1;
  int total_bytes = size * list->array->elem_size;
  memset(list->array->data, 0, total_bytes);
}

void destroy_IL(IList *list, void (*free_cb)(CDNode *node)) {
  if (list == NULL)
    return;
  destroy_CDL(list->list, free_cb);
  destroy_DA(list->array, NULL);
  free(list);
}

int size_IL(IList *list) {
  if (list == NULL || empty_IL(list))
    return 0;
  return size_CDL(list->list);
}

CDNode *front_IL(IList *list) {
  if (list == NULL)
    return NULL;
  return front_CDL(list->list);
}

CDNode *back_IL(IList *list) {
  if (list == NULL)
    return NULL;
  return back_CDL(list->list);
}

void print_IL(IList *list, void (*print_cb)(CDNode *node)) {
  if (list == NULL || print_cb == NULL)
    return;

  print_CDL(list->list, print_cb);
}

void push_back_IL(IList *list, CDNode *node, int (*get_val)(CDNode *node)) {
  if (list == NULL)
    return;
  push_back_CDL(list->list, node);
  update_array(list, node, get_val, node);
}

void push_front_IL(IList *list, CDNode *node, int (*get_val)(CDNode *node)) {
  if (list == NULL)
    return;
  push_front_CDL(list->list, node);
  update_array(list, node, get_val, node);
}

CDNode *pop_back_IL(IList *list, int (*get_val)(CDNode *node)) {
  if (list == NULL)
    return NULL;

  CDNode *n = pop_back_CDL(list->list);
  if (n == NULL)
    return NULL;
  update_array(list, n, get_val, NULL);
  return n;
}

CDNode *pop_front_IL(IList *list, int (*get_val)(CDNode *node)) {
  if (list == NULL)
    return NULL;
  CDNode *n = pop_front_CDL(list->list);
  if (n == NULL)
    return NULL;
  update_array(list, n, get_val, NULL);
  return n;
}

CDNode *find_by_pos_IL(IList *list, int pos) {
  if (list == NULL || pos < 0 || pos >= size_IL(list))
    return NULL;

  return find_CDL(list->list, pos);
}

CDNode *find_by_val_IL(IList *list, int val) {
  // [0, size)
  if (list == NULL || val < 0 || val > list->max_key)
    return NULL;

  return GET_DA(list->array, val, CDNode *);
}

void insert_IL(IList *list, CDNode *pre, CDNode *node,
               int (*get_val)(CDNode *node)) {
  insert_CDL(list->list, pre, node);
  update_array(list, node, get_val, node);
}

void erase_IL(IList *list, CDNode *node, int (*get_val)(CDNode *node),
              void (*free_cb)(CDNode *node)) {
  erase_CDL(list->list, node, free_cb);
  update_array(list, node, get_val, NULL);
}

void insert_by_val_IL(IList *list, CDNode *node, int val,
                      int (*get_val)(CDNode *node)) {
  if (list == NULL || val < 0 || val > list->max_key)
    return;

  CDNode *pre = find_by_val_IL(list, val);
  insert_IL(list, pre, node, get_val);
}

void erase_by_val_IL(IList *list, int val, int (*get_val)(CDNode *node),
                     void (*free_cb)(CDNode *node)) {

  if (list == NULL || get_val == NULL)
    return;
  CDNode *del = find_by_val_IL(list, val);
  erase_IL(list, del, get_val, free_cb);
}
