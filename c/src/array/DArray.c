#include "DArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DArray *newDArray(int cap, int elem_size) {
  if (elem_size == 0)
    return NULL;
  if (cap == 0)
    cap = 4;

  DArray *arr = malloc(sizeof(DArray));
  if (arr == NULL)
    return NULL;

  arr->data = malloc(cap * elem_size);
  if (arr->data == NULL) {
    free(arr);
    return NULL;
  }

  arr->len = 0;
  arr->capacity = cap;
  arr->elem_size = elem_size;
  return arr;
}

void *at_DA(DArray *arr, int pos) {
  return (char *)arr->data + pos * arr->elem_size;
}

bool empty_DA(DArray *arr) {
  if (arr == NULL)
    return true;

  return arr->len <= 0 ? true : false;
}

void clear_DA(DArray *arr, void (*free_cb)(void *elem)) {
  if (arr == NULL || empty_DA(arr))
    return;

  if (free_cb != NULL) {
    for (int i = 0; i < arr->len; ++i) {
      void *elem = at_DA(arr, i);
      free_cb(elem);
    }
  }
  arr->len = 0;
}

void destroy_DA(DArray *arr, void (*free_cb)(void *elem)) {
  if (arr == NULL)
    return;
  clear_DA(arr, free_cb);
  free(arr->data);
  free(arr);
}

int size_DA(DArray *arr) {
  if (arr == NULL)
    return 0;
  return arr->len;
}

int capacity_DA(DArray *arr) {
  if (arr == NULL)
    return 0;
  return arr->capacity;
}

void print_DA(DArray *arr, void (*print_cb)(void *elem)) {
  if (arr == NULL || print_cb == NULL) {
    putchar('\n');
    return;
  }

  int size = size_DA(arr);
  for (int i = 0; i < size; ++i) {
    void *elem = at_DA(arr, i);
    print_cb(elem);
  }
  putchar('\n');
}

void expand_DA(DArray *arr) {
  int new_cap = arr->capacity == 0 ? 4 : arr->capacity * 2;
  void *new_data = realloc(arr->data, new_cap * arr->elem_size);
  if (new_data == NULL)
    return;

  arr->data = new_data;
  arr->capacity = new_cap;
}

void push_back_DA(DArray *arr, void *elem) {
  if (arr == NULL || elem == NULL)
    return;

  if (size_DA(arr) == capacity_DA(arr))
    expand_DA(arr);

  void *dest = at_DA(arr, size_DA(arr));
  memcpy(dest, elem, arr->elem_size);
  ++arr->len;
}

void *pop_back_DA(DArray *arr) {
  if (arr == NULL || empty_DA(arr))
    return NULL;

  arr->len--;
  return at_DA(arr, size_DA(arr));
}
